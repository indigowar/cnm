#include "client_node.hpp"

#include <future>

using namespace Cnm;
using namespace Cnm::Connections;

ClientNode::ClientNode(Connection& con, std::shared_ptr<Node> node,
                       const std::shared_ptr<Utils::SleepWrapper>& sw)
    : ConnectionNode(con, std::move(node), sw) {}

ClientNode::~ClientNode() { abort(); }

void ClientNode::setNextNode(std::shared_ptr<ConnectionNode> next_node) {
  auto lock = makeLock();
  next = std::move(next_node);
}

std::shared_ptr<ConnectionNode> ClientNode::getNextNode() const noexcept {
  auto lock = makeLock();
  return next;
}

void ClientNode::setPreviousNode(std::shared_ptr<ConnectionNode> previousNode) {
  throw std::runtime_error("client node can not have a previous node");
}

std::shared_ptr<ConnectionNode> ClientNode::getPreviousNode() const noexcept {
  return nullptr;
}

void ClientNode::sendForward(Message&& msg) {
  auto& connection = getOwner();

  if (connection.isRequesting()) {
    auto lock = makeLock();

    if (next) {
      auto delay = std::chrono::milliseconds(connection.getSpeed());
      getSleepWrapper()->sleepFor(delay);

      (void)std::async(
          std::launch::async,
          [this](Message&& msg) { next->sendForward(std::move(msg)); },
          std::move(msg));
      return;
    }

    callAbort();
  }
}

void ClientNode::sendBackward(Message&& msg) {
  auto& connection = getOwner();

  if (connection.isServing()) {
    auto lock = makeLock();
    retrieved.push_back(msg);
  }
}

void ClientNode::abort() {
  auto lock = makeLock();
  callAbort();
}

void ClientNode::callAbort() {
  if (next) {
    next->abort();
    next.reset();
  }
  getOwner().abort();
}

const std::vector<Message>& ClientNode::getBuffer() const noexcept {
  return retrieved;
}
