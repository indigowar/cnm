#include "intermediate_node.hpp"

#include <future>

using namespace Cnm;
using namespace Cnm::Connections;

IntermediateNode::IntermediateNode(Connection& connection,
                                   std::shared_ptr<_Node> node,
                                   const Utils::SleepWrapper& sleeper,
                                   std::shared_ptr<ConnectionNode> prev,
                                   std::shared_ptr<ConnectionNode> next)
    : ConnectionNode(connection, std::move(node), sleeper),
      previous{std::move(prev)},
      next{std::move(next)} {}

IntermediateNode::~IntermediateNode() { this->abort(); }

void IntermediateNode::setNextNode(std::shared_ptr<ConnectionNode> new_next) {
  auto lock = makeLock();
  next = std::move(new_next);
}

std::shared_ptr<ConnectionNode> IntermediateNode::getNextNode() const noexcept {
  auto lock = makeLock();
  return next;
}

void IntermediateNode::setPreviousNode(
    std::shared_ptr<ConnectionNode> new_previous) {
  auto lock = makeLock();
  previous = std::move(new_previous);
}

std::shared_ptr<ConnectionNode> IntermediateNode::getPreviousNode()
    const noexcept {
  auto lock = makeLock();
  return previous;
}

void IntermediateNode::sendForward(Message&& msg) {
  auto lock = makeLock();
  if (next) {
    size_t net_speed = getOwner().getSpeed();

    getSleepWrapper().sleepFor(std::chrono::milliseconds(net_speed));
    auto result = std::async(
        std::launch::async,
        [this](Message&& msg) { next->sendForward(std::move(msg)); },
        std::move(msg));
  } else {
    callAbort();
  }
}

void IntermediateNode::sendBackward(Message&& msg) {
  auto lock = makeLock();
  if (previous) {
    size_t net_speed = getOwner().getSpeed();
    getSleepWrapper().sleepFor(std::chrono::milliseconds(net_speed));

    auto result = std::async(
        std::launch::async,
        [this](Message&& msg) { previous->sendBackward(std::move(msg)); },
        std::move(msg));
  } else {
    callAbort();
  }
}

void IntermediateNode::abort() {
  auto lock = makeLock();
  callAbort();
}

void IntermediateNode::callAbort() {
  getOwner().abort();

  if (previous) {
    previous->abort();
    previous.reset();
  }

  if (next) {
    next->abort();
    next.reset();
  }
}
