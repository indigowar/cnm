#ifndef HPP_CNM_CORE_TOPOLOGY_RING_RING_COMMUNICATOR_HPP
#define HPP_CNM_CORE_TOPOLOGY_RING_RING_COMMUNICATOR_HPP

#include "cnm/machine/communicator.hpp"
#include "cnm/topology/ring/ring.hpp"

namespace Cnm {

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

  result_t<ClientCtx> makeConnection(std::string_view address) override;

 private:
  Ring* ring;

  std::vector<HostInfo> getSpecificType(std::string_view type,
                                        bool filteR_unavailable = false);
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_RING_RING_COMMUNICATOR_HPP
