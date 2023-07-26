#include "intermediate_connection_node.hpp"

#include <iterator>

#include "cnm/connection/internal/base_connection_node.hpp"
#include "cnm/connection/internal/connection_chain.hpp"
#include "cnm/core/message.hpp"

using namespace Cnm;
using namespace Cnm::Connection;

IntermediateConnectionNode::IntermediateConnectionNode(
    ConnectionChain& connection, std::shared_ptr<_Node> node,
    std::shared_ptr<BaseConnectionNode> prev,
    std::shared_ptr<BaseConnectionNode> next)
    : BaseConnectionNode(connection, std::move(node)),
      previous{std::move(prev)},
      next{std::move(next)} {}

IntermediateConnectionNode::~IntermediateConnectionNode() { this->abort(); }

void IntermediateConnectionNode::setNextNode(
    std::shared_ptr<BaseConnectionNode> new_next) {
  auto lock = makeLock();
  next = std::move(new_next);
}

std::shared_ptr<BaseConnectionNode> IntermediateConnectionNode::getNextNode()
    const noexcept {
  auto lock = makeLock();
  return next;
}

void IntermediateConnectionNode::setPreviousNode(
    std::shared_ptr<BaseConnectionNode> new_previous) {
  auto lock = makeLock();
  previous = std::move(new_previous);
}

std::shared_ptr<BaseConnectionNode>
IntermediateConnectionNode::getPreviousNode() const noexcept {
  auto lock = makeLock();
  return previous;
}

void IntermediateConnectionNode::sendForward(MessageBatch&& msg) {
  auto lock = makeLock();
  if (next) next->sendForward(std::move(msg));
}

void IntermediateConnectionNode::sendBackward(MessageBatch&& msg) {
  auto lock = makeLock();
  if (previous) previous->sendBackward(std::move(msg));
}

void IntermediateConnectionNode::abort() {
  auto lock = makeLock();
  if (previous) previous = nullptr;
  if (next) next = nullptr;

  // TODO: IntermediateConnectionNode::abort() add call for deactivation of the
  // node.
}
