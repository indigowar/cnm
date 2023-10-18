#ifndef HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP
#define HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP

#include <map>

#include "cnm/topology/base/node_iterator.hpp"
#include "cnm/topology/ring/node.hpp"

namespace Cnm::Ring {

// Ring::Iterator - is an iterator for Ring topology.
class Iterator final {
  using MapIterator =
      std::map<std::string, std::shared_ptr<Ring::Node>>::iterator;

 public:
  explicit Iterator(MapIterator iterator) : iterator{iterator} {}

  ~Iterator() = default;

  Iterator(const Iterator&) = default;
  Iterator& operator=(const Iterator&) = default;

  Iterator(Iterator&&) = default;
  Iterator& operator=(Iterator&&) = default;

  std::shared_ptr<Node> operator*() const noexcept { return iterator->second; }

  Iterator& operator++() {
    iterator++;
    return *this;
  }

  Iterator& operator--() {
    iterator--;
    return *this;
  }

  template <NodeIterator I>
  bool operator==(const I& other) const {
    return iterator == other.iterator;
  }

  template <NodeIterator I>
  bool operator!=(const I& o) const {
    return iterator != o.iterator;
  }

 private:
  MapIterator iterator;
};

}  // namespace Cnm::Ring

#endif  // HPP_CNM_CORE_TOPOLOGY_RING_RING_ITERATOR_HPP