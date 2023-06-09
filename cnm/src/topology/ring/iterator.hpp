#ifndef HPP_CNM_LIB_TOPOLOGY_ITERATOR_HPP
#define HPP_CNM_LIB_TOPOLOGY_ITERATOR_HPP

#include <iterator>

#include "topology/ring/node.hpp"

namespace Cnm::Topology::Ring {

struct Iterator final {
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Node;
  using pointer = std::shared_ptr<Node>;
  using reference = Node&;

  explicit Iterator(pointer cursor) : cursor_{std::move(cursor)} {}

  reference operator*() const { return *cursor_; }

  pointer operator->() { return cursor_; }

  Iterator& operator++() {
    cursor_ = cursor_->getRight();
    return *this;
  }

  Iterator& operator++(int amount) {
    for (int i{}; i < amount; i++) cursor_ = cursor_->getRight();
    return *this;
  }

  friend bool operator==(const Iterator& a, const Iterator& b) {
    return a.cursor_ == b.cursor_;
  }

  friend bool operator!=(const Iterator& a, const Iterator& b) {
    return a.cursor_ != b.cursor_;
  }

 private:
  pointer cursor_;
};

struct ReverseIterator final {
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Node;
  using pointer = std::shared_ptr<Node>;
  using reference = Node&;

  explicit ReverseIterator(pointer p) : cursor_(std::move(p)) {}

  ReverseIterator& operator++() {
    cursor_ = cursor_->getLeft();
    return *this;
  }

  ReverseIterator& operator++(int amount) {
    for (int i{}; i < amount; i++) cursor_ = cursor_->getLeft();
    return *this;
  }

  friend bool operator==(const ReverseIterator& a, const ReverseIterator& b) {
    return a.cursor_ == b.cursor_;
  }

  friend bool operator!=(const ReverseIterator& a, const ReverseIterator& b) {
    return a.cursor_ != b.cursor_;
  }

 private:
  pointer cursor_;
};

}  // namespace Cnm::Topology::Ring

#endif  // HPP_CNM_LIB_TOPOLOGY_ITERATOR_HPP
