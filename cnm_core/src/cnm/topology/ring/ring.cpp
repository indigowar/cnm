#include "ring.hpp"

#include <ranges>

#include "cnm/topology/ring/ring_communicator.hpp"
#include "cnm/topology/ring/ring_iterator.hpp"

namespace Cnm {

void Ring::start() {
  signalNodes([](auto i) { i->start(); });
}

void Ring::stop() {
  signalNodes([](auto i) { i->stop(); });
}

void Ring::invoke() {
  signalNodes([](auto i) { i->invoke(); });
}

void Ring::freeze() {
  signalNodes([](auto i) { i->freeze(); });
}

result_t<HostInfo> Ring::addMachine(std::unique_ptr<Machine>&& machine) {
  std::unique_lock lock(mutex);
  auto info = generateHostInfo(machine->getType());
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
  auto host_info = generateHostInfo(name);
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
  auto node = std::make_shared<RingNode>(
      host_info, std::move(machine), std::make_unique<RingCommunicator>(this));
  nodes.emplace(node->getHostInfo().getAddress(), node);

  return result_t<HostInfo>::Ok(node->getHostInfo());
}

result_t<bool> Ring::validate() const noexcept {
  // TODO: Implement Ring::validate()

  auto current = nodes.begin()->second;
  std::set<std::string> s;

  for (; current; current = current->getNextNode()) {
    if (s.contains(current->getHostInfo().getAddress())) {
      if (current->getHostInfo().getAddress() == *s.begin()) {
        break;
      }
      return result_t<bool>::Err("");
    }
    s.insert(current->getHostInfo().getAddress());
  }
}

RingIterator Ring::begin() { return RingIterator(nodes.begin()->second); }

RingIterator Ring::end() { return RingIterator(nullptr); }

void Ring::signalNodes(std::function<void(std::shared_ptr<RingNode>&)> func) {
  auto only_nodes = nodes | std::views::values;
  std::for_each(only_nodes.begin(), only_nodes.end(), func);
}

}  // namespace Cnm
