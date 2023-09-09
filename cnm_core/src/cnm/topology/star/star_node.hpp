#ifndef HPP_CNM_CORE_TOPOLOGY_STAR_STAR_NODE_HPP
#define HPP_CNM_CORE_TOPOLOGY_STAR_STAR_NODE_HPP

#include "cnm/machine/communicator.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/topology/base/node.hpp"

namespace Cnm {

class Hub;

class StarNode final : public Node {
 public:
  StarNode(HostInfo, std::unique_ptr<Machine>&&,
           std::unique_ptr<Communicator>&&);

  ~StarNode();

 private:
  std::shared_ptr<Hub> hub;

  std::unique_ptr<Machine> machine;

  mutable std::mutex mutex;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_STAR_STAR_NODE_HPP
