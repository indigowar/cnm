#include "intermediate_node.hpp"

#include <iterator>

#include "cnm/connection/internal/connection.hpp"
#include "cnm/connection/internal/connection_node.hpp"
#include "cnm/core/message.hpp"

using namespace Cnm;
using namespace Cnm::Connections;

IntermediateNode::IntermediateNode(
    Connection& connection, std::shared_ptr<_Node> node,
    std::shared_ptr<ConnectionNode> prev,
    std::shared_ptr<ConnectionNode> next)
    : ConnectionNode(connection, std::move(node)),
      previous{std::move(prev)},
      next{std::move(next)} {}

IntermediateNode::~IntermediateNode() { this->abort(); }

void IntermediateNode::setNextNode(
    std::shared_ptr<ConnectionNode> new_next) {
  auto lock = makeLock();
  next = std::move(new_next);
}

std::shared_ptr<ConnectionNode> IntermediateNode::getNextNode()
    const noexcept {
  auto lock = makeLock();
  return next;
}

void IntermediateNode::setPreviousNode(
    std::shared_ptr<ConnectionNode> new_previous) {
  auto lock = makeLock();
  previous = std::move(new_previous);
}

std::shared_ptr<ConnectionNode> IntermediateNode::getPreviousNode() const noexcept {
  auto lock = makeLock();
  return previous;
}

void IntermediateNode::sendForward(Message&& msg) {
  auto lock = makeLock();
  if (next) next->sendForward(std::move(msg));
}

void IntermediateNode::sendBackward(Message&& msg) {
  auto lock = makeLock();
  if (previous) previous->sendBackward(std::move(msg));
}

void IntermediateNode::abort() {
  auto lock = makeLock();
  if (previous) previous = nullptr;
  if (next) next = nullptr;

  // TODO: IntermediateNode::abort() add call for deactivation of the
  // node.
}
