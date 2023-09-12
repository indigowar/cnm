#include "star_node.hpp"

#include "cnm/connection/server_ctx.hpp"
#include "cnm/machine/host_info.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/topology/star/star_hub.hpp"

namespace Cnm::Star {

Node::Node(HostInfo host_info, std::unique_ptr<Machine>&& m,
           const std::shared_ptr<Hub>& h)
    : hub{h}, machine{std::move(m)} {
  machine->setHostInfo(host_info);
  machine->setCommunicator(h->createCommunicator());
}

Node::~Node() = default;

HostInfo Node::getHostInfo() const noexcept { return machine->getHostInfo(); }

void Node::setHostInfo(HostInfo host_info) { machine->setHostInfo(host_info); }
std::vector<std::shared_ptr<Cnm::Node>> Node::getConnectedNodes()
    const noexcept {
  std::unique_lock lock(mutex);
  return {hub};
}

bool Node::isBusy() const noexcept {
  return machine->getServingLimit() == machine->getCurrentServingAmount();
}

bool Node::isServing() const noexcept {
  return machine->getCurrentServingAmount() != 0;
}

void Node::start() { machine->start(); }
void Node::stop() { machine->stop(); }
void Node::invoke() { machine->invoke(); }
void Node::freeze() { machine->freeze(); }

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
