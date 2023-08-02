#include "server_node.hpp"

#include <future>

using namespace Cnm;
using namespace Cnm::Connections;

ServerNode::ServerNode(Connection &connection, std::shared_ptr<Node> node,
                       const Utils::SleepWrapper &sw)
    : ConnectionNode(connection, std::move(node), sw) {}

ServerNode::~ServerNode() { this->abort(); }

void ServerNode::setNextNode(std::shared_ptr<ConnectionNode> new_next) {
  auto lock = makeLock();
  throw std::runtime_error("server connection node cannot have next node.");
}

std::shared_ptr<ConnectionNode> ServerNode::getNextNode() const noexcept {
  return nullptr;
}

void ServerNode::setPreviousNode(std::shared_ptr<ConnectionNode> new_previous) {
  auto lock = makeLock();
  previous = std::move(new_previous);
}

std::shared_ptr<ConnectionNode> ServerNode::getPreviousNode() const noexcept {
  return previous;
}

void ServerNode::sendForward(Message &&msg) {
  auto lock = makeLock();
  if (getOwner().isRequesting()) {
    auto delay = std::chrono::milliseconds(getOwner().getSpeed());
    getSleepWrapper().sleepFor(delay);
    retrieved.push_back(msg);
  }
}

void ServerNode::sendBackward(Cnm::Message &&msg) {
  auto lock = makeLock();

  if (getOwner().isServing()) {
    if (previous) {
      auto delay = std::chrono::milliseconds(getOwner().getSpeed());
      getSleepWrapper().sleepFor(delay);

      auto result = std::async(
          [this](Message &&msg) { previous->sendBackward(std::move(msg)); },
          std::move(msg));

    } else {
      callAbort();
    }
  }
}

void ServerNode::abort() {
  auto lock = makeLock();
  callAbort();
}

const std::vector<Message> &ServerNode::getBuffer() const noexcept {
  return retrieved;
}

void ServerNode::callAbort() {
  getOwner().abort();

  if (previous) {
    previous->abort();
    previous.reset();
  }
}
