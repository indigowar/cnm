#ifndef HPP_CNM_CORE_MACHINE_COMMUNICATOR_HPP
#define HPP_CNM_CORE_MACHINE_COMMUNICATOR_HPP

#include <string_view>
#include <vector>

#include "cnm/connection/client_ctx.hpp"
#include "cnm/machine/host_info.hpp"

namespace Cnm {

// Communicator - is an interface for communication with the network by machine.
class Communicator {
 public:
  virtual ~Communicator() = default;

  virtual void setNode(std::shared_ptr<Node>) = 0;

  virtual std::vector<HostInfo> getOfficeEquipments(
      bool filter_unavailable = false) = 0;

  virtual std::vector<HostInfo> getServers(bool filter_unavailable = false) = 0;

  virtual std::vector<HostInfo> getPCs() = 0;

  virtual void disconnect(HostInfo) = 0;

  virtual result_t<ClientCtx> makeConnection(std::string address) = 0;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_COMMUNICATOR_HPP
