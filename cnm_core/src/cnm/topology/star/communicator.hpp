#ifndef HPP_CNM_CORE_TOPOLOGY_STAR_COMMUNICATOR_HPP
#define HPP_CNM_CORE_TOPOLOGY_STAR_COMMUNICATOR_HPP

#include "cnm/machine/communicator.hpp"
#include "cnm/machine/office_equipment/office_equipment.hpp"
#include "cnm/machine/personal_computer/personal_computer.hpp"
#include "cnm/machine/server/server.hpp"
#include "cnm/topology/star/hub.hpp"

namespace Cnm::Star {

class Communicator final : public Cnm::Communicator {
 public:
  explicit Communicator(Hub*);

  void setNode(Cnm::Node* node) override;

  std::vector<HostInfo> getOfficeEquipments(bool filter_unavailable) override {
    return getSpecificType(OfficeEquipment::Type, filter_unavailable);
  }

  std::vector<HostInfo> getServers(bool filter_unavailable) override {
    return getSpecificType(Server::Type, filter_unavailable);
  }

  std::vector<HostInfo> getPCs() override {
    return getSpecificType(PersonalComputer::Type);
  }

  void disconnect(HostInfo info) override;

  result_t<ClientCtx> makeConnection(std::string address) override;

 private:
  std::vector<HostInfo> getSpecificType(std::string_view type,
                                        bool filter_unavailable = false);

  Hub* hub;
  Cnm::Node* node;
};

}  // namespace Cnm::Star

#endif  // HPP_CNM_CORE_TOPOLOGY_STAR_COMMUNICATOR_HPP
