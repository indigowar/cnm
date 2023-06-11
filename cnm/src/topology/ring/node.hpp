#ifndef HPP_CNM_LIB_TOPOLOGY_RING_NODE_HPP
#define HPP_CNM_LIB_TOPOLOGY_RING_NODE_HPP

#include <memory>

#include "machines/machine.hpp"
#include "topology/common/node.hpp"

namespace Cnm::Topology::Ring {

class Node : public Common::Node {
 public:
  Node(std::unique_ptr<Machines::Machine>&& machine, std::shared_ptr<Node> left,
       std::shared_ptr<Node> right, std::string_view address)
      : Common::Node(address, std::move(machine)),

        left_{std::move(left)},
        right_{std::move(right)} {}

  std::shared_ptr<Node> getLeft() const noexcept { return left_; }
  std::shared_ptr<Node> getRight() const noexcept { return right_; }

  void setRight(std::shared_ptr<Node> v) { right_ = v; }
  void setLeft(std::shared_ptr<Node> v) { left_ = v; }

 private:
  std::shared_ptr<Node> left_;
  std::shared_ptr<Node> right_;
};
}  // namespace Cnm::Topology::Ring

#endif  // HPP_CNM_LIB_TOPOLOGY_RING_NODE_HPP
