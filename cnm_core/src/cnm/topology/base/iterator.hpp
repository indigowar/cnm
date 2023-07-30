#ifndef HPP_CNM_CORE_TOPOLOGY_BASE_ITERATOR_HPP
#define HPP_CNM_CORE_TOPOLOGY_BASE_ITERATOR_HPP

#include <functional>
#include <memory>

#include "cnm/topology/base/node.hpp"

namespace Cnm {

// Iterator is generic interface for all iterators, that are used as a return
// value from Topology::begin() and Topology::end() methods.
class Iterator {
  virtual std::shared_ptr<Node> operator*() const = 0;
  virtual Iterator& operator++() = 0;
  virtual Iterator& operator--() = 0;
  virtual bool operator!=(const Iterator& other) const = 0;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_BASE_ITERATOR_HPP
