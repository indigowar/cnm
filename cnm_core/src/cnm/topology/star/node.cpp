#include "node.hpp"

#include <utility>

namespace Cnm::Star {

Node::Node(HostInfo hi, std::unique_ptr<Machine>&& m,
           const std::shared_ptr<Hub>& h)
    : machine(std::move(m)), hub(h) {
  machine->setHostInfo(std::move(hi));
  // TODO: Star::Node::Node() set machine's communicator to the created by Hub.
}

Node::~Node() = default;

void Node::start() { machine->start(); }

void Node::stop() { machine->stop(); }

void Node::invoke() { machine->invoke(); }

void Node::freeze() { machine->freeze(); }

HostInfo Node::getHostInfo() const noexcept { return machine->getHostInfo(); }

bool Node::isServing() const noexcept {
  return machine->getCurrentServingAmount() > 0;
}

bool Node::isBusy() const noexcept {
  return machine->getCurrentServingAmount() == machine->getServingLimit();
}

std::vector<std::shared_ptr<Cnm::Node>> Node::getConnectedNodes()
    const noexcept {
  // TODO: make Star::Node::getConnectedNodes to return the Hub.
  return {};
}

void Node::serve(ServerCtx&& ctx) { machine->serve(std::move(ctx)); }

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

}  // namespace Cnm::Star