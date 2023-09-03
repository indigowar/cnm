#ifndef HPP_CNM_CORE_TOPOLOGY_RING_RING_COMMUNICATOR_HPP
#define HPP_CNM_CORE_TOPOLOGY_RING_RING_COMMUNICATOR_HPP

#include "cnm/machine/communicator.hpp"
#include "cnm/machine/office_equipment/office_equipment.hpp"
#include "cnm/machine/personal_computer/personal_computer.hpp"
#include "cnm/machine/server/server.hpp"
#include "cnm/topology/ring/ring.hpp"

namespace Cnm {

class RingCommunicator final : public Communicator {
 public:
  explicit RingCommunicator(Ring* ring);

  void setNode(std::shared_ptr<Node>) override;

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

  result_t<ClientCtx> makeConnection(std::string address) override;

 private:
  std::vector<HostInfo> getSpecificType(std::string_view type,
                                        bool filteR_unavailable = false);

  result_t<std::vector<std::shared_ptr<RingNode>>> findShortestPath(
      const std::string& from, const std::string& to);

  Ring* ring;
  std::shared_ptr<Node> node;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_RING_RING_COMMUNICATOR_HPP
