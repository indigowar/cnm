#ifndef HPP_CNM_CORE_TOPOLOGY_TOPOLOGY_HPP
#define HPP_CNM_CORE_TOPOLOGY_TOPOLOGY_HPP

#include <memory>

#include "cnm/core/object.hpp"
#include "cnm/machine/host_info.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

// Topology is base class for different topologies.
class Topology : public Object {
 public:
  virtual result_t<HostInfo> addMachine(std::unique_ptr<Machine>&&,
                                        std::string_view addr = "") = 0;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_TOPOLOGY_HPP
