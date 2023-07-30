#ifndef HPP_CNM_CORE_TOPOLOGY_BASE_TOPOLOGY_HPP
#define HPP_CNM_CORE_TOPOLOGY_BASE_TOPOLOGY_HPP

#include <memory>

#include "cnm/machine/host_info.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

class Topology {
 public:
  virtual result_t<HostInfo> addMachine(std::unique_ptr<Machine>&& machine,
                                        std::string_view host_name = "",
                                        std::string_view address = "") = 0;

  virtual size_t getAmoutOfConnections() const noexcept = 0;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_BASE_TOPOLOGY_HPP
