#include "star.hpp"

namespace Cnm::Star {

Star::Star() {
  hub = std::make_shared<Hub>(this, HostInfo::generate("Hub", 127, 0, 0, 1));
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
  return hub->addMachine(std::move(m), info);
}

result_t<HostInfo> Star::addMachineWithName(std::unique_ptr<Machine>&& m,
                                            std::string_view name) {
  auto info = generateFreeHostInfo(std::string{name});
  return hub->addMachine(std::move(m), info);
}

result_t<Cnm::HostInfo> Star::addMachineWithAddress(
    std::unique_ptr<Machine>&& m, std::string_view address) {
  HostInfo info(std::string{m->getType()}, std::string{address});
  return hub->addMachine(std::move(m), info);
}

result_t<bool> Star::deleteMachine(Cnm::HostInfo info) {
  return hub->deleteMachine(info);
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

std::shared_ptr<Cnm::Star::Hub> Star::getHub() const noexcept { return hub; }

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

}  // namespace Cnm::Star