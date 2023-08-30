#ifndef HPP_CNM_CORE_TOPOLOGY_BASE_NODE_ITERATOR_HPP
#define HPP_CNM_CORE_TOPOLOGY_BASE_NODE_ITERATOR_HPP

#include <concepts>
#include <functional>
#include <memory>

#include "cnm/topology/base/node.hpp"

namespace Cnm {

template <typename T>
concept NodeIterator = std::bidirectional_iterator<T> && requires(T a, T b) {
  { a.operator*() } -> std::same_as<std::shared_ptr<Node>>;
  { a.operator==(b) } -> std::same_as<bool>;
  { a.operator!=(b) } -> std::same_as<bool>;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_BASE_NODE_ITERATOR_HPP
