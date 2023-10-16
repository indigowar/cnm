#ifndef HPP_CNM_CORE_TOPOLOGY_STAR_STAR_HPP
#define HPP_CNM_CORE_TOPOLOGY_STAR_STAR_HPP

#include "cnm/topology/base/topology.hpp"
#include "cnm/topology/star/hub.hpp"

namespace Cnm::Star {

class Star final : public Topology {
 public:
  Star();

  std::shared_ptr<Hub> getHub() noexcept;

  void start() override;

  void stop() override;

  void invoke() override;

  void freeze() override;

  Object::Status getStatus() const noexcept override {
    return hub->getStatus();
  }

  result_t<HostInfo> addMachine(std::unique_ptr<Machine>&& ptr) override;

  result_t<HostInfo> addMachine(std::unique_ptr<Machine>&& ptr,
                                HostInfo info) override;

  result_t<HostInfo> addMachineWithName(std::unique_ptr<Machine>&& ptr,
                                        std::string_view name) override;

  result_t<HostInfo> addMachineWithAddress(std::unique_ptr<Machine>&& ptr,
                                           std::string_view address) override;

  result_t<bool> deleteMachine(HostInfo info) override;

  result_t<bool> validate() const noexcept override;

  std::string_view getType() const noexcept override;

  std::vector<std::shared_ptr<Cnm::Node>>::iterator begin();
  std::vector<std::shared_ptr<Cnm::Node>>::iterator end();

  std::unique_lock<std::mutex> makeLock() const noexcept;

  size_t getNetworkSpeed() const noexcept override;

  void setNetworkSpeed(size_t speed) override;

 private:
  HostInfo generateFreeHostInfo(std::string name);

  std::shared_ptr<Hub> hub;

  std::vector<std::shared_ptr<Cnm::Node>> nodes;

  size_t network_speed{0};

  mutable std::mutex mutex;
};

}  // namespace Cnm::Star

#endif  // HPP_CNM_CORE_TOPOLOGY_STAR_STAR_HPP
