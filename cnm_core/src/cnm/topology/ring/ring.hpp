#ifndef HPP_CNM_CORE_TOPOLOGY_RING_RING_HPP
#define HPP_CNM_CORE_TOPOLOGY_RING_RING_HPP

#include <map>

#include "cnm/machine/office_equipment/office_equipment.hpp"
#include "cnm/machine/personal_computer/personal_computer.hpp"
#include "cnm/machine/server/server.hpp"
#include "cnm/topology/base/topology.hpp"
#include "cnm/topology/ring/ring_node.hpp"

namespace Cnm {

class Ring final : public Topology {
 public:
  // TODO: add the proper constructor.
  Ring() = default;

  ~Ring() override;

  void start() override;

  void stop() override;

  void invoke() override;

  void freeze() override;

  result_t<HostInfo> addMachine(std::unique_ptr<Machine>&&) override;

  result_t<HostInfo> addMachine(std::unique_ptr<Machine>&&, HostInfo) override;

  result_t<HostInfo> addMachineWithName(std::unique_ptr<Machine>&&,
                                        std::string_view) override;

  result_t<HostInfo> addMachineWithAddress(std::unique_ptr<Machine>&&,
                                           std::string_view) override;

  result_t<bool> deleteMachine(HostInfo) override;

  result_t<bool> validate() const noexcept override;

  std::string_view getType() const noexcept override { return "Ring"; }

  NodeIterator begin() override;
  NodeIterator end() override;

 private:
  class RingCommunicator final : public Communicator {
   public:
    explicit RingCommunicator(Ring* ring);

    std::vector<HostInfo> getOfficeEquipments(
        bool filter_unavailable = false) override {
      return getSpecificType(OfficeEquipment::Type, filter_unavailable);
    }

    std::vector<HostInfo> getServers(bool filter_unavailable = false) override {
      return getSpecificType(Server::Type, filter_unavailable);
    }

    std::vector<HostInfo> getPCs() override {
      return getSpecificType(PersonalComputer::Type);
    }

    void disconnect(HostInfo) override;

    void makeConnection(std::string_view address) override;

   private:
    Ring* ring;

    std::vector<HostInfo> getSpecificType(std::string_view type,
                                          bool filteR_unavailable = false);
  };

  result_t<HostInfo> createNode(HostInfo, std::unique_ptr<Machine>&&);

  HostInfo generateHostInfo(std::string_view name);

  std::map<std::string_view, std::shared_ptr<RingNode>> nodes;

  std::shared_ptr<RingCommunicator> communicator;

  mutable std::mutex mutex;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_RING_RING_HPP
