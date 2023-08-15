#ifndef HPP_CNM_CORE_TOPOLOGY_BASE_NODE_ITERATOR_HPP
#define HPP_CNM_CORE_TOPOLOGY_BASE_NODE_ITERATOR_HPP

#include <functional>
#include <memory>

#include "cnm/topology/base/node.hpp"

namespace Cnm {

// NodeIterator - is the base class for topology's iterators.
class NodeIterator {
 public:
  virtual std::shared_ptr<Node> operator*() const { return nullptr; }

  virtual NodeIterator& operator++() { return *this; }
  virtual NodeIterator& operator--() { return *this; }

  bool operator==(const NodeIterator& other) const {
    auto this_node = *(*this);
    auto other_node = *other;

    if (this_node == other_node) {
      return true;
    }

    if (!this_node || !other_node) {
      return false;
    }

    return this_node->getHostInfo() == other_node->getHostInfo();
  }

  bool operator!=(const NodeIterator& other) const { return !(*this == other); }
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_BASE_NODE_ITERATOR_HPP
