#include "node.hpp"

namespace Cnm {

Node::Node(HostInfo host_info, std::unique_ptr<Machine>&& m,
           const std::shared_ptr<Communicator>& communicator)
    : machine{std::move(m)} {
  machine->setHostInfo(host_info);
  machine->setCommunicator(communicator);
}

Node::~Node() = default;

void Node::start() { machine->start(); }

void Node::stop() { machine->stop(); }

void Node::invoke() { machine->invoke(); }

void Node::freeze() { machine->freeze(); }

HostInfo Node::getHostInfo() const noexcept { return machine->getHostInfo(); }

void Node::setHostInfo(HostInfo host_info) { machine->setHostInfo(host_info); }

std::vector<std::shared_ptr<Node>> Node::getConnectedNodes() const noexcept {
  return {};
}

bool Node::isServing() const noexcept {
  return machine->getCurrentServingAmount() > 0;
}

void Node::serve(ServerCtx&& ctx) {
  // TODO: implement Node::serve()
}

}  // namespace Cnm