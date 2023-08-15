#ifndef HPP_CNM_CORE_TOPOLOGY_BASE_NODE_HPP
#define HPP_CNM_CORE_TOPOLOGY_BASE_NODE_HPP

#include "cnm/machine/host_info.hpp"

namespace Cnm {

// Node is the base class for nodes in network.
// Every topology should implement it's own node, that derives from this class.
class Node {
 public:
  explicit Node(HostInfo host_info) : host_info{host_info} {}

  virtual ~Node() = default;

  [[nodiscard]] HostInfo getHostInfo() const noexcept { return host_info; }

  void setHostInfo(HostInfo info) { host_info = info; }

 private:
  HostInfo host_info;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_BASE_NODE_HPP
