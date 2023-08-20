#ifndef HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP
#define HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP

#include "cnm/topology/base/node_iterator.hpp"
#include "cnm/topology/ring/ring_node.hpp"

namespace Cnm {

class RingIterator final : public NodeIterator {
 public:
  explicit RingIterator(std::shared_ptr<RingNode> node)
      : node{std::move(node)} {}

  std::shared_ptr<Node> operator*() const override { return node; }

  NodeIterator& operator++() override {
    if (node) {
      node = node->getNextNode();
    }
    return *this;
  }

  NodeIterator& operator--() {
    if (node) {
      node = node->getPreviousNode();
    }
    return *this;
  }

 private:
  std::shared_ptr<RingNode> node;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP