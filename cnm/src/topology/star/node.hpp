#ifndef HPP_CNM_LIB_TOPOLOGY_START_NODE_HPP
#define HPP_CNM_LIB_TOPOLOGY_START_NODE_HPP

#include "machines/machine.hpp"
#include "topology/common/exceptions.hpp"
#include "topology/common/node.hpp"

namespace Cnm::Topology::Star {

class Hub;

class Node : public Common::Node {
 public:
  Node(std::unique_ptr<Machines::Machine>&& machine, std::string_view address,
       std::shared_ptr<Hub> hub)
      : Common::Node(address, std::move(machine)), hub_{std::move(hub)} {}

 private:
  std::shared_ptr<Hub> hub_;
};

}  // namespace Cnm::Topology::Star

#endif  // HPP_CNM_LIB_TOPOLOGY_START_NODE_HPP
