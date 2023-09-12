#ifndef HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP
#define HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP

#include "cnm/topology/base/node_iterator.hpp"
#include "cnm/topology/ring/node.hpp"

namespace Cnm::Ring {

// Ring::Iterator - is an iterator for Ring topology.
class Iterator final {
 public:
  explicit Iterator(std::shared_ptr<Node> node) : node{std::move(node)} {}

  ~Iterator() = default;

  Iterator(const Iterator&) = default;
  Iterator& operator=(const Iterator&) = default;

  Iterator(Iterator&&) = default;
  Iterator& operator=(Iterator&&) = default;

  std::shared_ptr<Node> operator*() const noexcept { return node; }

  Iterator& operator++() {
    if (node) {
      node = node->getNextNode();
    }
    return *this;
  }

  Iterator& operator--() {
    if (node) {
      node = node->getPreviousNode();
    }
    return *this;
  }

  template <NodeIterator I>
  bool operator==(const I& other) const {
    auto t = this->operator*();
    auto o = this->operator*();

    // equal pointers - same node.
    if (t == o) {
      return true;
    }

    // if one of pointers is nullptr and other one is not.
    if (!t || !o) {
      return false;
    }

    return t->getHostInfo() == o->getHostInfo();
  }

  template <NodeIterator I>
  bool operator!=(const I& o) const {
    return !this->operator==(o);
  }

 private:
  std::shared_ptr<Node> node;
};

}  // namespace Cnm::Ring

#endif  // HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP