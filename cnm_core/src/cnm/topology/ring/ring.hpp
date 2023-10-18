#ifndef HPP_CNM_CORE_TOPOLOGY_RING_RING_HPP
#define HPP_CNM_CORE_TOPOLOGY_RING_RING_HPP

#include <map>

#include "cnm/topology/base/topology.hpp"
#include "cnm/topology/ring/iterator.hpp"
#include "cnm/topology/ring/node.hpp"

namespace Cnm::Ring {

class Communicator;

class Ring final : public Topology {
 public:
  Ring() = default;

  ~Ring() override = default;

  void start() override;

  void stop() override;

  void invoke() override;

  void freeze() override;

  Object::Status getStatus() const noexcept override;

  result_t<HostInfo> addMachine(std::unique_ptr<Machine>&&) override;

  result_t<HostInfo> addMachine(std::unique_ptr<Machine>&&, HostInfo) override;

  result_t<HostInfo> addMachineWithName(std::unique_ptr<Machine>&&,
                                        std::string_view) override;

  result_t<HostInfo> addMachineWithAddress(std::unique_ptr<Machine>&&,
                                           std::string_view) override;

  result_t<bool> deleteMachine(HostInfo) override;

  result_t<bool> validate() const noexcept override;

  std::string_view getType() const noexcept override { return TYPE; }

  size_t getNetworkSpeed() const noexcept override;

  void setNetworkSpeed(size_t speed) override;

  [[nodiscard]] Iterator begin();
  [[nodiscard]] Iterator end();

  static constexpr std::string_view TYPE = "ring_topology";

 private:
  friend class Cnm::Ring::Communicator;

  result_t<HostInfo> createNode(HostInfo, std::unique_ptr<Machine>&&);

  HostInfo generateHostInfo(const std::string& name);

  void signalNodes(std::function<void(std::shared_ptr<Node>&)>);

  std::map<std::string, std::shared_ptr<Node>> nodes;

  Object::Status status{NotInitialized};

  size_t network_speed{0};

  mutable std::mutex mutex;
};

}  // namespace Cnm::Ring

#endif  // HPP_CNM_CORE_TOPOLOGY_RING_RING_HPP
