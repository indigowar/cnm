#include "client_connection_node.hpp"

#include <stdexcept>

#include "cnm/connection/internal/base_connection_node.hpp"
#include "cnm/connection/internal/connection_chain.hpp"

using namespace Cnm;
using namespace Cnm::Connection;

ClientConnectionNode::ClientConnectionNode(
    ConnectionChain& connection, std::shared_ptr<_Node> node,
    std::shared_ptr<BaseConnectionNode> next)
    : BaseConnectionNode(connection, std::move(node)), next{std::move(next)} {}

ClientConnectionNode::~ClientConnectionNode() {}

void ClientConnectionNode::setNextNode(
    std::shared_ptr<BaseConnectionNode> next) {
  auto lock = makeLock();
  next = std::move(next);
}

std::shared_ptr<BaseConnectionNode> ClientConnectionNode::getNextNode()
    const noexcept {
  return next;
}

void ClientConnectionNode::setPreviousNode(
    std::shared_ptr<BaseConnectionNode>) {
  throw std::runtime_error("client node can't have previous node");
}

std::shared_ptr<BaseConnectionNode> ClientConnectionNode::getPreviousNode()
    const noexcept {
  return nullptr;
}
