#include "ring.hpp"

#include <ranges>
#include <utility>

#include "cnm/topology/ring/communicator.hpp"
#include "cnm/topology/ring/iterator.hpp"

namespace Cnm::Ring {

void Ring::start() {
  std::unique_lock lock(mutex);
  status = Running;
  signalNodes([](auto i) { i->start(); });
}

void Ring::stop() {
  std::unique_lock lock(mutex);
  status = Dead;
  signalNodes([](auto i) { i->stop(); });
}

void Ring::invoke() {
  std::unique_lock lock(mutex);
  status = Running;
  signalNodes([](auto i) { i->invoke(); });
}

void Ring::freeze() {
  std::unique_lock lock(mutex);
  status = Freezed;
  signalNodes([](auto i) { i->freeze(); });
}

Object::Status Ring::getStatus() const noexcept {
  std::unique_lock lock(mutex);
  return status;
}

result_t<HostInfo> Ring::addMachine(std::unique_ptr<Machine>&& machine) {
  std::unique_lock lock(mutex);
  auto info = generateHostInfo(std::string(machine->getType()));
  return createNode(info, std::move(machine));
}

result_t<HostInfo> Ring::addMachine(std::unique_ptr<Machine>&& machine,
                                    HostInfo host_info) {
  std::unique_lock lock(mutex);
  return createNode(host_info, std::move(machine));
}

result_t<HostInfo> Ring::addMachineWithAddress(
    std::unique_ptr<Machine>&& machine, std::string_view address) {
  std::unique_lock lock(mutex);
  HostInfo host_info(machine->getType(), address);
  return createNode(host_info, std::move(machine));
}

result_t<HostInfo> Ring::addMachineWithName(std::unique_ptr<Machine>&& machine,
                                            std::string_view name) {
  std::unique_lock lock(mutex);
  auto host_info = generateHostInfo(std::string(name));
  return createNode(host_info, std::move(machine));
}

result_t<bool> Ring::deleteMachine(HostInfo info) {
  std::unique_lock lock(mutex);

  if (!nodes.contains(info.getAddress())) {
    return result_t<bool>::Err("there is no node with given address.");
  }

  auto node = nodes.at(info.getAddress());
  node->getPreviousNode()->setNextNode(nullptr);
  node->getNextNode()->setPreviousNode(nullptr);

  size_t i = std::erase_if(nodes, [&info](auto& item) -> bool {
    auto& [address, node] = item;
    return node->getHostInfo() == info;
  });

  if (i == 0) {
    return result_t<bool>::Err("there is no node with given HostInfo.");
  }

  return result_t<bool>::Ok(true);
}

result_t<HostInfo> Ring::createNode(HostInfo host_info,
                                    std::unique_ptr<Machine>&& machine) {
  if (nodes.contains(host_info.getAddress())) {
    return result_t<HostInfo>::Err("given address is already in use.");
  }
  auto node = std::make_shared<Node>(host_info, std::move(machine),
                                     std::make_unique<Communicator>(this));
  nodes.emplace(node->getHostInfo().getAddress(), node);

  return result_t<HostInfo>::Ok(node->getHostInfo());
}

result_t<bool> Ring::validate() const noexcept {
  std::unique_lock lock(mutex);

  std::set<std::string> found{};

  auto node = nodes.begin()->second;
  auto addr = node->getHostInfo().getAddress();
  do {
    if (found.contains(addr)) {
      return result_t<bool>::Ok(false);
    }
    node = node->getNextNode();
    addr = node->getHostInfo().getAddress();
  } while (nodes.begin()->first != addr);

  return result_t<bool>::Ok(found.size() == nodes.size());
}

Iterator Ring::begin() { return Iterator(nodes.begin()); }

Iterator Ring::end() { return Iterator(nodes.end()); }

void Ring::signalNodes(std::function<void(std::shared_ptr<Node>&)> func) {
  auto only_nodes = nodes | std::views::values;
  std::for_each(only_nodes.begin(), only_nodes.end(), std::move(func));
}

HostInfo Ring::generateHostInfo(const std::string& name) {
  HostInfo hi{};
  do {
    hi = HostInfo::generate(name);
  } while (!nodes.contains(hi.getAddress()));
  return hi;
}

size_t Ring::getNetworkSpeed() const noexcept {
  std::unique_lock lock(mutex);
  return network_speed;
}

void Ring::setNetworkSpeed(size_t speed) {
  std::unique_lock lock(mutex);
  network_speed = speed;
}

}  // namespace Cnm::Ring
