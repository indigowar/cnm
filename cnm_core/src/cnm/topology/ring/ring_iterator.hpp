//#ifndef HPP_CNM_CORE_RING_RING_ITERATOR_HPP
//#define HPP_CNM_CORE_RING_RING_ITERATOR_HPP
//
//#include <memory>
//
//#include "cnm/topology/base/node.hpp"
//#include "cnm/topology/base/node_iterator.hpp"
//#include "cnm/topology/ring/ring_node.hpp"
//
//namespace Cnm {
//
//class RingIterator final : public NodeIterator {
// public:
//  explicit RingIterator(std::shared_ptr<RingNode> node)
//      : node{std::move(node)} {}
//
//  std::shared_ptr<Node> operator*() const override { return node; }
//
//  NodeIterator& operator++() override {
//    node = std::move(node->getNextNode());
//    return *this;
//  }
//
//  NodeIterator& operator--() override {
//    node = std::move(node->getPreviousNode());
//    return *this;
//  }
//
// private:
//  std::shared_ptr<RingNode> node;
//};
//
//}  // namespace Cnm
//
//#endif  // HPP_CNM_CORE_RING_RING_ITERATOR_HPP
