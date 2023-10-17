#include "connection.hpp"

#include <memory>

#include "cnm/connection/client_ctx.hpp"
#include "cnm/connection/internal/connection_node.hpp"
#include "cnm/connection/internal/intermediate_node.hpp"
#include "cnm/connection/server_ctx.hpp"
#include "cnm/topology/base/node.hpp"
#include "cnm/utils/result.hpp"
#include "cnm/utils/sleep_wrapper.hpp"

using namespace Cnm;
namespace Cons = Connections;

Connection::Connection(size_t net_speed,
                       const std::vector<std::shared_ptr<Node>> &nodes)
    : net_speed{net_speed}, called_aborted{}, status{Status::Created} {
  if (nodes.size() < 2) {
    throw std::runtime_error("can't create connection with less than 2 nodes.");
  }

  auto sleep_wrapper = std::move(std::make_shared<Utils::SleepWrapper>());

  client_node =
      std::make_shared<Cons::ClientNode>(*this, nodes.at(0), sleep_wrapper);
  std::shared_ptr<Cons::ConnectionNode> prev_node = client_node;

  for (auto it = nodes.begin() + 1; it != nodes.end() - 1; it++) {
    auto node = *it;
    auto intermediate = std::make_shared<Cons::IntermediateNode>(
        *this, node, sleep_wrapper, prev_node);
    intermediate->setPreviousNode(prev_node);
    prev_node->setNextNode(intermediate);
    prev_node = intermediate;
  }

  server_node =
      std::make_shared<Cons::ServerNode>(*this, nodes.back(), sleep_wrapper);
  server_node->setPreviousNode(prev_node);
  prev_node->setNextNode(server_node);
}

void Connection::abort() {
  auto lock = makeLock();
  if (called_aborted) {
    return;
  }

  callAbort();
}

bool Connection::isRequesting() const noexcept {
  auto lock = makeLock();
  return status == Status::Requesting;
}

bool Connection::isServing() const noexcept {
  auto lock = makeLock();
  return status == Status::Serving;
}

bool Connection::isAborted() const noexcept {
  auto lock = makeLock();
  return status == Status::Aborted && called_aborted;
}

std::unique_lock<std::mutex> Connection::makeLock() const noexcept {
  return std::unique_lock(mutex);
}

size_t Connection::getSpeed() const noexcept {
  auto lock = makeLock();
  return net_speed;
}

HostInfo Connection::getClientHostInfo() const noexcept {
  return client_node->getNetworkNode()->getHostInfo();
}

HostInfo Connection::getServerHostInfo() const noexcept {
  return server_node->getNetworkNode()->getHostInfo();
}

void Connection::changeStatus(Connection::Status s) {
  if (!called_aborted) {
    status = s;
  }
}

void Connection::callAbort() {
  status = Status::Aborted;
  called_aborted = true;

  client_node.reset();
  server_node.reset();
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

// using namespace Cnm;
//
// Connection::~Connection() { abort(); }
//
// void Connection::abort() {
//   auto lock = makeLock();
//   callAbort();
// }
//
// bool Connection::isRequesting() const noexcept {
//   auto lock = makeLock();
//   return current_direction == Connection::SendingDirection::ToServer;
// }
//
// bool Connection::isServing() const noexcept {
//   auto lock = makeLock();
//   return current_direction == Connection::SendingDirection::ToClient;
// }
//
// bool Connection::isAborted() const noexcept {
//   auto lock = makeLock();
//   return current_direction == Connection::SendingDirection::None;
// }
//
// std::unique_lock<std::mutex> Connection::makeLock() const noexcept {
//   return std::unique_lock(mutex);
// }
//
// size_t Connection::getSpeed() const noexcept { return speed; }
//
// void Connection::stopRequesting() {
//   auto lock = makeLock();
//   if (current_direction == Connection::SendingDirection::ToServer) {
//     current_direction = Connection::SendingDirection::ToClient;
//   }
// }
//
// void Connection::stopServing() {
//   auto lock = makeLock();
//   if (current_direction == Connection::SendingDirection::ToClient) {
//     current_direction = Connection::SendingDirection::None;
//   }
// }
//
// result_t<ClientCtx> Connection::createClientContext() {
//   auto result = client_ctx_builder.build(this, client_node);
//
//   if (result.isErr()) {
//     return result_t<ClientCtx>::Err(result.unwrapErr());
//   }
//   return result_t<ClientCtx>::Ok(
//       std::make_unique<ClientContext>(result.unwrap()));
// }
//
// result_t<ServerCtx> Connection::createServerContext() {
//   auto result = server_ctx_builder.build(this, server_node);
//
//   if (result.isErr()) {
//     return result_t<ServerCtx>::Err(result.unwrapErr());
//   }
//   return result_t<ServerCtx>::Ok(
//       std::make_unique<ServerContext>(result.unwrap()));
// }
//
// void Connection::callAbort() {
//   if (!is_aborted) {
//     is_aborted = true;
//     client_node.reset();
//     server_node.reset();
//     nodes.resize(0);
//     nodes.shrink_to_fit();
//   }
// }
//
// HostInfo Connection::getClientHostInfo() const noexcept {
//   return client_node->getNetworkNode()->getHostInfo();
// }
//
// HostInfo Connection::getServerHostInfo() const noexcept {
//   return server_node->getNetworkNode()->getHostInfo();
// }
