#include "ring_node.hpp"

#include <string_view>

namespace Cnm {

RingNode::RingNode(Cnm::HostInfo host_info, std::unique_ptr<Machine>&& m,
                   std::unique_ptr<Communicator>&& communicator)
    : machine{std::move(m)}, previous_node{}, next_node{} {
  machine->setHostInfo(host_info);
  machine->setCommunicator(std::move(communicator));
}

RingNode::~RingNode() = default;

HostInfo RingNode::getHostInfo() const noexcept {
  return machine->getHostInfo();
}

void RingNode::setHostInfo(HostInfo host_info) {
  machine->setHostInfo(host_info);
}

std::vector<std::shared_ptr<Node>> RingNode::getConnectedNodes()
    const noexcept {
  std::unique_lock lock(mutex);

  if (next_node && previous_node) {
    return {next_node, previous_node};
  }

  if (next_node) {
    return {next_node};
  }

  if (previous_node) {
    return {previous_node};
  }

  return {};
}

bool RingNode::isBusy() const noexcept {
  return machine->getServingLimit() == machine->getCurrentServingAmount();
}

bool RingNode::isServing() const noexcept {
  return machine->getCurrentServingAmount() != 0;
}

std::shared_ptr<RingNode> RingNode::getPreviousNode() const noexcept {
  std::unique_lock lock(mutex);
  return previous_node;
}

void RingNode::setPreviousNode(std::shared_ptr<RingNode> previous) {
  std::unique_lock lock(mutex);
  previous_node = std::move(previous);
}

void RingNode::setNextNode(std::shared_ptr<RingNode> next) {
  std::unique_lock lock(mutex);
  next_node = std::move(next);
}

std::shared_ptr<RingNode> RingNode::getNextNode() const noexcept {
  std::unique_lock lock(mutex);
  return next_node;
}

void RingNode::start() { machine->start(); }
void RingNode::stop() { machine->stop(); }
void RingNode::invoke() { machine->invoke(); }
void RingNode::freeze() { machine->freeze(); }

void RingNode::serve(std::unique_ptr<ServerContext>&& ctx) {
  machine->serve(std::move(ctx));
}

std::string_view RingNode::getType() const noexcept {
  return machine->getType();
}

void RingNode::attachConnectionNode(Connections::ConnectionNode* ptr) {
  if (!connection_nodes.contains(ptr)) {
    connection_nodes.emplace(ptr);
  }
}
void RingNode::detachConnectionNode(Connections::ConnectionNode* ptr) {
  if (connection_nodes.contains(ptr)) {
    connection_nodes.erase(ptr);
  }
}

std::vector<ConnectionInfo> RingNode::getConnections() const noexcept {
  std::vector<ConnectionInfo> result{};
  std::transform(connection_nodes.begin(), connection_nodes.end(),
                 std::back_inserter(result),
                 [](Connections::ConnectionNode* ptr) -> ConnectionInfo {
                   return {ptr->getOwner().getClientHostInfo(),
                           ptr->getOwner().getServerHostInfo()};
                 });
  return result;
}

}  // namespace Cnm
