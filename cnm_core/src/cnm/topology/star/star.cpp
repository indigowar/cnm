#include "star.hpp"

namespace Cnm::Star {

Star::Star() {
  hub = std::make_shared<Hub>(this, HostInfo::generate("Hub", 127, 0, 0, 1));
  nodes.emplace_back(hub);
}

void Star::start() { hub->start(); }

void Star::stop() { hub->stop(); }

void Star::invoke() { hub->invoke(); }

void Star::freeze() { hub->freeze(); }

result_t<Cnm::HostInfo> Star::addMachine(std::unique_ptr<Machine>&& m) {
  auto info = generateFreeHostInfo(std::string{m->getType()});
  return hub->addMachine(std::move(m), info);
}

result_t<Cnm::HostInfo> Star::addMachine(std::unique_ptr<Machine>&& m,
                                         HostInfo info) {
  auto result = hub->addMachine(std::move(m), info);
  {
    auto lock = makeLock();
    nodes.emplace_back(*hub->nodes.rbegin());
  }
  return result;
}

result_t<HostInfo> Star::addMachineWithName(std::unique_ptr<Machine>&& m,
                                            std::string_view name) {
  auto info = generateFreeHostInfo(std::string{name});
  auto result = hub->addMachine(std::move(m), info);
  {
    auto lock = makeLock();
    nodes.emplace_back(*hub->nodes.rbegin());
  }
  return result;
}

result_t<Cnm::HostInfo> Star::addMachineWithAddress(
    std::unique_ptr<Machine>&& m, std::string_view address) {
  HostInfo info(std::string{m->getType()}, std::string{address});
  auto result = hub->addMachine(std::move(m), info);
  {
    auto lock = makeLock();
    nodes.emplace_back(*hub->nodes.rbegin());
  }
  return result;
}

result_t<bool> Star::deleteMachine(Cnm::HostInfo info) {
  auto result = hub->deleteMachine(info);
  if (result.isOk()) {
    //      auto lock = makeLock();
    std::erase_if(nodes, [this, &info](const auto& i) {
      return i->getHostInfo() == info;
    });
  }
  return result;
}

result_t<bool> Star::validate() const noexcept {
  std::unique_lock lock(hub->mutex);

  for (const auto& i : *hub) {
    auto conns = i->getConnectedNodes();
    if (conns.size() != 1) {
      return result_t<bool>::Ok(false);
    }
    if (conns.at(0) != hub) {
      return result_t<bool>::Ok(false);
    }
  }

  auto count = std::distance(hub->begin(), hub->end());
  return result_t<bool>::Ok(count == hub->getConnectedNodes().size());
}

std::string_view Star::getType() const noexcept { return std::string_view(); }

std::shared_ptr<Cnm::Star::Hub> Star::getHub() noexcept { return hub; }

HostInfo Star::generateFreeHostInfo(std::string name) {
  std::unique_lock lock(hub->mutex);
  HostInfo info(name, "");
  auto finder = [&info](const auto& i) {
    return i->getHostInfo().getAddress() == info.getAddress();
  };
  do {
    info = HostInfo::generate(info.getAddress());
  } while (std::find_if(hub->nodes.begin(), hub->nodes.end(), finder) !=
           hub->nodes.end());
  return info;
}

std::vector<std::shared_ptr<Cnm::Node>>::iterator Star::begin() {
  return nodes.begin();
}

std::vector<std::shared_ptr<Cnm::Node>>::iterator Star::end() {
  return nodes.end();
}

std::unique_lock<std::mutex> Star::makeLock() const noexcept {
  return std::unique_lock(mutex);
}

size_t Star::getNetworkSpeed() const noexcept {
  std::unique_lock lock(mutex);
  return network_speed;
}

void Star::setNetworkSpeed(size_t speed) {
  std::unique_lock lock(mutex);
  network_speed = speed;
}

}  // namespace Cnm::Star