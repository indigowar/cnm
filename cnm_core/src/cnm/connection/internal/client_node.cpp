#include <stdexcept>

#include "client_node.hpp"
#include "cnm/connection/internal/connection.hpp"
#include "cnm/connection/internal/connection_node.hpp"

using namespace Cnm;
using namespace Cnm::Connections;

ClientNode::ClientNode(
    Connection& connection, std::shared_ptr<_Node> node,
    std::shared_ptr<ConnectionNode> next)
    : ConnectionNode(connection, std::move(node)), next{std::move(next)} {}

ClientNode::~ClientNode() {}

void ClientNode::setNextNode(
    std::shared_ptr<ConnectionNode> next) {
  auto lock = makeLock();
  next = std::move(next);
}

std::shared_ptr<ConnectionNode> ClientNode::getNextNode()
    const noexcept {
  return next;
}

void ClientNode::setPreviousNode(
    std::shared_ptr<ConnectionNode>) {
  throw std::runtime_error("client node can't have previous node");
}

std::shared_ptr<ConnectionNode> ClientNode::getPreviousNode()
    const noexcept {
  return nullptr;
}
