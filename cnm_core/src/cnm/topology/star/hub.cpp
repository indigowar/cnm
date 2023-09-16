#include "hub.hpp"

#include "cnm/topology/base/node.hpp"
#include "cnm/topology/star/communicator.hpp"
#include "cnm/topology/star/node.hpp"

namespace Cnm::Star {

Hub::Hub(Cnm::HostInfo hi) : host_info(std::move(hi)) {}

void Hub::start() {}

void Hub::stop() {}

void Hub::invoke() {}

void Hub::freeze() {}

HostInfo Hub::getHostInfo() const noexcept {
  std::unique_lock lock(mutex);
  return host_info;
}

void Hub::setHostInfo(Cnm::HostInfo info) {
  std::unique_lock lock(mutex);
  host_info = std::move(info);
}

bool Hub::isServing() const noexcept { return false; }

bool Hub::isBusy() const noexcept { return false; }

std::vector<std::shared_ptr<Cnm::Node>> Hub::getConnectedNodes()
    const noexcept {
  std::unique_lock lock(mutex);
  std::vector<std::shared_ptr<Cnm::Node>> result{};
  std::copy(nodes.begin(), nodes.end(), std::back_inserter(result));
  return result;
}

void Hub::serve(std::unique_ptr<ServerContext> &&ctx) { ctx->abort(); }

std::string_view Hub::getType() const noexcept { return "Hub"; }

void Hub::attachConnectionNode(Connections::ConnectionNode *node) {
  std::unique_lock lock(mutex);

  if (!connection_nodes.contains(node)) {
    connection_nodes.emplace(node);
  }
}

void Hub::detachConnectionNode(Connections::ConnectionNode *node) {
  std::unique_lock lock(mutex);

  if (connection_nodes.contains(node)) {
    connection_nodes.erase(node);
  }
}

std::vector<ConnectionInfo> Hub::getConnections() const noexcept {
  std::vector<ConnectionInfo> result{};
  std::transform(connection_nodes.begin(), connection_nodes.end(),
                 std::back_inserter(result),
                 [](Connections::ConnectionNode *ptr) -> ConnectionInfo {
                   return {ptr->getOwner().getClientHostInfo(),
                           ptr->getOwner().getServerHostInfo()};
                 });
  return result;
}

std::unique_ptr<Communicator> Hub::createCommunicator() {
  // TODO: Add here a creation of Cnm::Star::Communicator.
  return nullptr;
}

result_t<HostInfo> Hub::addMachine(std::unique_ptr<Machine> &&machine,
                                   HostInfo host_info) {
  std::unique_lock lock(mutex);

  auto predicate = [&host_info](const auto &i) {
    return i->getHostInfo() == host_info;
  };

  if (std::find_if(nodes.begin(), nodes.end(), predicate) != nodes.end()) {
    return result_t<HostInfo>::Err("node with given host info already exists.");
  }

  // TODO: replace this with given the star->hub member.
  auto node =
      std::make_shared<Star::Node>(host_info, std::move(machine), nullptr);

  nodes.emplace(node);

  return result_t<HostInfo>::Ok(node->getHostInfo());
}

result_t<bool> Hub::deleteMachine(HostInfo hi) {
  std::unique_lock lock(mutex);

  auto it = std::find_if(nodes.begin(), nodes.end(),
                         [&hi](auto &i) { return i->getHostInfo() == hi; });
  if (it == nodes.end()) {
    return result_t<bool>::Err("node with given host is not found.");
  }

  nodes.erase(*it);
  return result_t<bool>::Ok(true);
}

}  // namespace Cnm::Star