#include "connection.hpp"

#include <memory>

#include "cnm/connection/client_ctx.hpp"
#include "cnm/connection/internal/client_node.hpp"
#include "cnm/connection/internal/connection_node.hpp"
#include "cnm/connection/server_ctx.hpp"
#include "cnm/topology/base/node.hpp"
#include "cnm/utils/result.hpp"

using namespace Cnm;

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
