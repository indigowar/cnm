#ifndef HPP_CNM_CORE_MACHINE_INTERACTOR_HPP
#define HPP_CNM_CORE_MACHINE_INTERACTOR_HPP

#include <string_view>
#include <vector>

#include "cnm/machine/host_info.hpp"

namespace Cnm {

class Interactor {
 public:
  virtual std::vector<HostInfo> getOfficeEquipments(
      bool filter_unavailable = false) = 0;

  virtual std::vector<HostInfo> getServers(bool filter_unavailable = false) = 0;

  virtual std::vector<HostInfo> getPCs() = 0;

  virtual void disconnect(HostInfo) = 0;

  virtual void makeConnection(std::string_view address) = 0;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_INTERACTOR_HPP
