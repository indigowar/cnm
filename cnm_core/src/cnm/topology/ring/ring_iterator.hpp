#ifndef HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP
#define HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP

#include "cnm/topology/base/node_iterator.hpp"
#include "cnm/topology/ring/ring_node.hpp"

namespace Cnm {

// RingIterator - is an iterator for Ring topology.
class RingIterator final {
 public:
  explicit RingIterator(std::shared_ptr<RingNode> node)
      : node{std::move(node)} {}

  ~RingIterator() = default;

  RingIterator(const RingIterator&) = default;
  RingIterator& operator=(const RingIterator&) = default;

  RingIterator(RingIterator&&) = default;
  RingIterator& operator=(RingIterator&&) = default;

  std::shared_ptr<Node> operator*() const noexcept { return node; }

  RingIterator& operator++() {
    if (node) {
      node = node->getNextNode();
    }
    return *this;
  }

  RingIterator& operator--() {
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
  std::shared_ptr<RingNode> node;
};

class RingReverseIterator final {
 public:
  explicit RingReverseIterator(std::shared_ptr<RingNode> node)
      : node{std::move(node)} {}

  ~RingReverseIterator() = default;

  RingReverseIterator(const RingReverseIterator&) = default;
  RingReverseIterator& operator=(const RingReverseIterator&) = default;

  RingReverseIterator(RingReverseIterator&&) = default;
  RingReverseIterator& operator=(RingReverseIterator&&) = default;

  std::shared_ptr<Node> operator*() const noexcept { return node; }

  RingReverseIterator& operator++() {
    if (node) {
      node = node->getPreviousNode();
    }
    return *this;
  }

  RingReverseIterator& operator--() {
    if (node) {
      node = node->getNextNode();
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
  std::shared_ptr<RingNode> node;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP