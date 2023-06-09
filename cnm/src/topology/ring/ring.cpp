#include "ring.hpp"

#include <memory>

#include "topology/common/exceptions.hpp"
#include "topology/ring/iterator.hpp"
#include "topology/ring/node.hpp"

using Topology = Cnm::Topology::Ring::Topology;
using Node = Cnm::Topology::Ring::Node;
using Iterator = Cnm::Topology::Ring::Iterator;
using ReverseIterator = Cnm::Topology::Ring::ReverseIterator;

using namespace Cnm::Connection;
using namespace Cnm::Communication;
using namespace Cnm::Machines;

size_t Topology::getNetSpeed() const noexcept { return net_speed_; }

void Topology::setNetSpeed(size_t value) { net_speed_ = value; }

std::string_view Topology::addMachine(std::unique_ptr<Machine> machine) {
  auto ptr = std::make_shared<Node>(std::move(machine), nullptr, nullptr,
                                    generateFreeAddress());

  auto head_left = head_->getLeft();
  ptr->setLeft(head_left);
  ptr->setRight(head_);
  head_->setLeft(ptr);

  head_ = ptr;

  return head_->getAddress();
}

std::string_view Topology::addMachine(
    std::unique_ptr<Machines::Machine> machine, std::string_view host) {
  auto newNode =
      std::make_shared<Node>(std::move(machine), nullptr, nullptr, host);

  auto head_left = head_->getLeft();
  newNode->setLeft(head_left);
  newNode->setRight(head_);
  head_->setLeft(newNode);

  head_ = newNode;

  return head_->getAddress();
}

void Topology::removeMachine(std::string_view address) {
  for (auto &item : *this) {
    if (item.getAddress() == address) {
      auto left = item.getLeft();
      auto right = item.getRight();

      left->setRight(right);
      right->setLeft(left);

      return;
    }
  }

  throw Exceptions::AddressNotFound(address, getType());
}

void Topology::stop() {
  for (auto &it : *this) it.abort();
}

Iterator Topology::begin() { return Iterator(head_); }

Iterator Topology::end() { return Iterator(nullptr); }

ReverseIterator Topology::rbegin() { return ReverseIterator(head_); }

ReverseIterator Topology::rend() { return ReverseIterator(nullptr); }
