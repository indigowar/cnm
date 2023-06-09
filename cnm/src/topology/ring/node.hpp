#ifndef HPP_CNM_LIB_TOPOLOGY_RING_NODE_HPP
#define HPP_CNM_LIB_TOPOLOGY_RING_NODE_HPP

#include <memory>

#include "machines/machine.hpp"

namespace Cnm::Topology::Ring {

struct Node final {
  Node(std::unique_ptr<Machines::Machine>&& machine, std::shared_ptr<Node> left,
       std::shared_ptr<Node> right, std::string_view address)
      : machine_{std::move(machine)},
        left_{std::move(left)},
        right_{std::move(right)},
        address_{address} {
    // todo: add here call of machine_->setHost(address_);
  }

  [[nodiscard]] std::string_view getName() const noexcept {
    return "machine_name";  // TODO: replace with machine_->getName()
  }

  [[nodiscard]] std::string_view getType() const noexcept {
    return "machine_type";  // TODO: replace with machine_->getType()
  }

  [[nodiscard]] std::string_view getAddress() const noexcept {
    return address_;
  }

  void serve(std::unique_ptr<Connection::ServerCtx>&& ctx) {
    machine_->serve(std::move(ctx));
  }

  [[nodiscard]] std::shared_ptr<Node> getLeft() const noexcept { return left_; }

  void setLeft(std::shared_ptr<Node> left) { left_ = left; }

  [[nodiscard]] std::shared_ptr<Node> getRight() const noexcept {
    return right_;
  }

  void setRight(std::shared_ptr<Node> right) { right_ = right; }

  void abort() {}

 private:
  std::string_view address_;
  std::unique_ptr<Machines::Machine> machine_;

  std::shared_ptr<Node> left_;
  std::shared_ptr<Node> right_;
};

}  // namespace Cnm::Topology::Ring

#endif  // HPP_CNM_LIB_TOPOLOGY_RING_NODE_HPP
