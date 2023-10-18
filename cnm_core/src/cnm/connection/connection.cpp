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
    notifier.notify_all();
  }
}

void Connection::callAbort() {
  status = Status::Aborted;
  called_aborted = true;

  client_node.reset();
  server_node.reset();
}

void Connection::signalAccept() {
  auto lock = makeLock();
  if (status == Status::Created) {
    changeStatus(Status::Requesting);
  }
}

void Connection::signalComplete() {
  auto lock = makeLock();
  if (status != Status::Aborted && status != Status::Complete) {
    changeStatus(Status::Complete);
  }
}

void Connection::signalStopRequest() {
  auto lock = makeLock();
  if (status != Status::Aborted && status != Status::Complete &&
      status == Status::Requesting) {
    changeStatus(Status::Serving);
  }
}

Connections::ServerT Connection::getServerNode() { return server_node; }

Connections::ClientT Connection::getClientNode() { return client_node; }

result_t<bool> Connection::waitForStatusChange(Connection::Status s) {
  auto lock = makeLock();
  if (status == s) {
    return result_t<bool>::Ok(true);
  }

  if (status > s) {
    return result_t<bool>::Err("connection never will get into this state");
  }

  if (status == Status::Aborted) {
    return result_t<bool>::Err("connection is aborted");
  }

  notifier.wait(lock, [this, &s] {
    return status == s || status == Status::Aborted ||
           status == Status::Complete;
  });

  if (s == status) {
    return result_t<bool>::Ok(true);
  } else {
    return result_t<bool>::Err("status has changed to different.");
  }
}

result_t<bool> Connection::waitForRequesting() {
  return waitForStatusChange(Status::Requesting);
}

result_t<bool> Connection::waitForServing() {
  return waitForStatusChange(Status::Serving);
}

result_t<bool> Connection::waitForComplete() {
  return waitForStatusChange(Status::Complete);
}
