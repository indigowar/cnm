#include "star_node.hpp"

#include "cnm/connection/server_ctx.hpp"
#include "cnm/machine/host_info.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/topology/star/star_hub.hpp"

namespace Cnm {

StarNode::StarNode(HostInfo host_info, std::unique_ptr<Machine>&& m,
                   const std::shared_ptr<Hub>& h)
    : hub{h}, machine{std::move(m)} {
  machine->setHostInfo(host_info);
  machine->setCommunicator(h->createCommunicator());
}

StarNode::~StarNode() = default;

HostInfo StarNode::getHostInfo() const noexcept {
  return machine->getHostInfo();
}

void StarNode::setHostInfo(HostInfo host_info) {
  machine->setHostInfo(host_info);
}
std::vector<std::shared_ptr<Node>> StarNode::getConnectedNodes()
    const noexcept {
  std::unique_lock lock(mutex);
  return {hub};
}

bool StarNode::isBusy() const noexcept {
  return machine->getServingLimit() == machine->getCurrentServingAmount();
}

bool StarNode::isServing() const noexcept {
  return machine->getCurrentServingAmount() != 0;
}

void StarNode::start() { machine->start(); }
void StarNode::stop() { machine->stop(); }
void StarNode::invoke() { machine->invoke(); }
void StarNode::freeze() { machine->freeze(); }

void StarNode::serve(ServerCtx&& ctx) { machine->serve(std::move(ctx)); }

std::string_view StarNode::getType() const noexcept {
  return machine->getType();
}

void StarNode::attachConnectionNode(Connections::ConnectionNode* ptr) {
  if (!connection_nodes.contains(ptr)) {
    connection_nodes.emplace(ptr);
  }
}
void StarNode::detachConnectionNode(Connections::ConnectionNode* ptr) {
  if (connection_nodes.contains(ptr)) {
    connection_nodes.erase(ptr);
  }
}

std::vector<ConnectionInfo> StarNode::getConnections() const noexcept {
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
