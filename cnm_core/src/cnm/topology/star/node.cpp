#include "node.hpp"

#include <utility>

#include "cnm/topology/star/hub.hpp"

namespace Cnm::Star {

Node::Node(HostInfo hi, std::unique_ptr<Machine>&& m,
           const std::shared_ptr<Hub>& h)
    : machine(std::move(m)), hub(h) {
  machine->setHostInfo(std::move(hi));
  auto c = h->createCommunicator();
  c->setNode(this);
  machine->setCommunicator(std::move(c));
}

Node::~Node() = default;

void Node::start() { machine->start(); }

void Node::stop() { machine->stop(); }

void Node::invoke() { machine->invoke(); }

void Node::freeze() { machine->freeze(); }

HostInfo Node::getHostInfo() const noexcept { return machine->getHostInfo(); }

void Node::setHostInfo(Cnm::HostInfo info) { machine->setHostInfo(info); }

bool Node::isServing() const noexcept {
  return machine->getCurrentServingAmount() > 0;
}

bool Node::isBusy() const noexcept {
  return machine->getCurrentServingAmount() == machine->getServingLimit();
}

std::vector<std::shared_ptr<Cnm::Node>> Node::getConnectedNodes()
    const noexcept {
  return {hub};
}

void Node::serve(ServerCtx&& ctx) { machine->serve(std::move(ctx)); }

std::string_view Node::getType() const noexcept { return machine->getType(); }

void Node::attachConnectionNode(Connections::ConnectionNode* ptr) {
  std::unique_lock lock(mutex);

  if (!connection_nodes.contains(ptr)) {
    connection_nodes.emplace(ptr);
  }
}
void Node::detachConnectionNode(Connections::ConnectionNode* ptr) {
  std::unique_lock lock(mutex);

  if (connection_nodes.contains(ptr)) {
    connection_nodes.erase(ptr);
  }
}

std::vector<ConnectionInfo> Node::getConnections() const noexcept {
  std::unique_lock lock(mutex);

  std::vector<ConnectionInfo> result{};
  std::transform(connection_nodes.begin(), connection_nodes.end(),
                 std::back_inserter(result),
                 [](Connections::ConnectionNode* ptr) -> ConnectionInfo {
                   return {ptr->getOwner().getClientHostInfo(),
                           ptr->getOwner().getServerHostInfo()};
                 });
  return result;
}

Object::Status Node::getStatus() const noexcept { return machine->getStatus(); }

}  // namespace Cnm::Star
