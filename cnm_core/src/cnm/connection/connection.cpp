#include "connection.hpp"

#include <memory>

#include "cnm/connection/client_ctx.hpp"
#include "cnm/connection/internal/client_node.hpp"
#include "cnm/connection/internal/connection_node.hpp"
#include "cnm/connection/internal/intermediate_node.hpp"
#include "cnm/connection/internal/server_node.hpp"
#include "cnm/connection/server_ctx.hpp"
#include "cnm/topology/base/node.hpp"
#include "cnm/utils/result.hpp"
#include "cnm/utils/sleep_wrapper.hpp"

using namespace Cnm;

Connection::Connection(
    size_t net_speed,
    const std::vector<std::shared_ptr<Cnm::Node>>& networkNodes)
    : speed{net_speed}, nodes(networkNodes.size()) {
  if (networkNodes.size() < 2) {
    throw std::runtime_error("can't create connection with less than 2 nodes.");
  }

  sleep_wrapper = std::make_shared<Utils::SleepWrapper>();

  // create the first node - client node.
  client_node = std::make_shared<Connections::ClientNode>(
      *this, networkNodes.front(), sleep_wrapper);

  // create intermediate nodes for the connection.
  std::shared_ptr<Connections::ConnectionNode> previous = client_node;
  for (size_t i{1}; i < networkNodes.size() - 1; i++) {
    auto current = std::make_shared<Connections::IntermediateNode>(
        *this, networkNodes[i], sleep_wrapper, previous);
    current->setPreviousNode(previous);
    previous->setNextNode(current);

    previous = current;
  }

  // create the last - server node.
  nodes.back() = server_node = std::make_shared<Connections::ServerNode>(
      *this, networkNodes.back(), sleep_wrapper);
  server_node->setPreviousNode(previous);
  previous->setNextNode(server_node);
}

Connection::~Connection() { abort(); }

void Connection::abort() {
  auto lock = makeLock();
  callAbort();
}

bool Connection::isRequesting() const noexcept {
  auto lock = makeLock();
  return current_direction == Connection::SendingDirection::ToServer;
}

bool Connection::isServing() const noexcept {
  auto lock = makeLock();
  return current_direction == Connection::SendingDirection::ToClient;
}

bool Connection::isAborted() const noexcept {
  auto lock = makeLock();
  return current_direction == Connection::SendingDirection::None;
}

std::unique_lock<std::mutex> Connection::makeLock() const noexcept {
  return std::unique_lock(mutex);
}

size_t Connection::getSpeed() const noexcept { return speed; }

void Connection::stopRequesting() {
  auto lock = makeLock();
  if (current_direction == Connection::SendingDirection::ToServer) {
    current_direction = Connection::SendingDirection::ToClient;
  }
}

void Connection::stopServing() {
  auto lock = makeLock();
  if (current_direction == Connection::SendingDirection::ToClient) {
    current_direction = Connection::SendingDirection::None;
  }
}

result_t<ClientCtx> Connection::createClientContext() {
  auto result = client_ctx_builder.build(this, client_node);

  if (result.isErr()) {
    return result_t<ClientCtx>::Err(result.unwrapErr());
  }
  return result_t<ClientCtx>::Ok(
      std::make_unique<ClientContext>(result.unwrap()));
}

result_t<ServerCtx> Connection::createServerContext() {
  auto result = server_ctx_builder.build(this, server_node);

  if (result.isErr()) {
    return result_t<ServerCtx>::Err(result.unwrapErr());
  }
  return result_t<ServerCtx>::Ok(
      std::make_unique<ServerContext>(result.unwrap()));
}

void Connection::callAbort() {
  if (!is_aborted) {
    is_aborted = true;
    client_node.reset();
    server_node.reset();
    nodes.resize(0);
    nodes.shrink_to_fit();
  }
}

HostInfo Connection::getClientHostInfo() const noexcept {
  return client_node->getNetworkNode()->getHostInfo();
}

HostInfo Connection::getServerHostInfo() const noexcept {
  return server_node->getNetworkNode()->getHostInfo();
}
