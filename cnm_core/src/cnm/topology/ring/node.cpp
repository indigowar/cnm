#include "node.hpp"

#include <string_view>

namespace Cnm::Ring {

Node::Node(Cnm::HostInfo host_info, std::unique_ptr<Machine>&& m,
           std::unique_ptr<Communicator>&& communicator)
    : machine{std::move(m)}, previous_node{}, next_node{} {
  machine->setHostInfo(host_info);
  machine->setCommunicator(std::move(communicator));
}

Node::~Node() = default;

HostInfo Node::getHostInfo() const noexcept { return machine->getHostInfo(); }

void Node::setHostInfo(HostInfo host_info) { machine->setHostInfo(host_info); }

std::vector<std::shared_ptr<Cnm::Node>> Node::getConnectedNodes()
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

bool Node::isBusy() const noexcept {
  return machine->getServingLimit() == machine->getCurrentServingAmount();
}

bool Node::isServing() const noexcept {
  return machine->getCurrentServingAmount() != 0;
}

std::shared_ptr<Node> Node::getPreviousNode() const noexcept {
  std::unique_lock lock(mutex);
  return previous_node;
}

void Node::setPreviousNode(std::shared_ptr<Node> previous) {
  std::unique_lock lock(mutex);
  previous_node = std::move(previous);
}

void Node::setNextNode(std::shared_ptr<Node> next) {
  std::unique_lock lock(mutex);
  next_node = std::move(next);
}

std::shared_ptr<Node> Node::getNextNode() const noexcept {
  std::unique_lock lock(mutex);
  return next_node;
}

void Node::start() { machine->start(); }
void Node::stop() { machine->stop(); }
void Node::invoke() { machine->invoke(); }
void Node::freeze() { machine->freeze(); }

void Node::serve(std::unique_ptr<ServerContext>&& ctx) {
  machine->serve(std::move(ctx));
}

std::string_view Node::getType() const noexcept { return machine->getType(); }

void Node::attachConnectionNode(Connections::ConnectionNode* ptr) {
  if (!connection_nodes.contains(ptr)) {
    connection_nodes.emplace(ptr);
  }
}
void Node::detachConnectionNode(Connections::ConnectionNode* ptr) {
  if (connection_nodes.contains(ptr)) {
    connection_nodes.erase(ptr);
  }
}

std::vector<ConnectionInfo> Node::getConnections() const noexcept {
  std::vector<ConnectionInfo> result{};
  std::transform(connection_nodes.begin(), connection_nodes.end(),
                 std::back_inserter(result),
                 [](Connections::ConnectionNode* ptr) -> ConnectionInfo {
                   return {ptr->getOwner().getClientHostInfo(),
                           ptr->getOwner().getServerHostInfo()};
                 });
  return result;
}

}  // namespace Cnm::Ring
