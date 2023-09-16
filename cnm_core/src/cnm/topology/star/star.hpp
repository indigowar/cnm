#ifndef HPP_CNM_CORE_TOPOLOGY_STAR_STAR_HPP
#define HPP_CNM_CORE_TOPOLOGY_STAR_STAR_HPP

#include "cnm/topology/base/topology.hpp"
#include "cnm/topology/star/hub.hpp"

namespace Cnm::Star {

class Star final : public Topology {
 public:
  Star();

  std::shared_ptr<Hub> getHub() const noexcept;

  void start() override;

  void stop() override;

  void invoke() override;

  void freeze() override;

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

 private:
  HostInfo generateFreeHostInfo(std::string name);

  std::shared_ptr<Hub> hub;
};

}  // namespace Cnm::Star

#endif  // HPP_CNM_CORE_TOPOLOGY_STAR_STAR_HPP
