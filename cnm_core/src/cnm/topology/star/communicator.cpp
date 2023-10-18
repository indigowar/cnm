#include "communicator.hpp"

#include <spdlog/spdlog.h>

#include <ranges>

#include "cnm/connection/connection.hpp"
#include "cnm/connection/ctx_factory.hpp"
#include "cnm/topology/star/hub.hpp"
#include "cnm/topology/star/star.hpp"

namespace Cnm::Star {

Communicator::Communicator(Hub *h) : hub{h}, node{} {}

void Communicator::setNode(Cnm::Node *n) { node = n; }

void Communicator::disconnect(HostInfo info) {
  if (node->getHostInfo() != info) {
    return;
  }
  std::unique_lock lock(hub->mutex);

  auto node_it =
      std::find_if(hub->nodes.begin(), hub->nodes.end(),
                   [&info](const auto &i) { return i->getHostInfo() == info; });

  if (node_it == hub->nodes.end()) {
    return;
  }

  hub->nodes.erase(*node_it);
}

result_t<ClientCtx> Communicator::makeConnection(std::string address) {
  std::unique_lock lock(hub->mutex);

  auto server_it = std::find_if(
      hub->nodes.begin(), hub->nodes.end(), [&address](const auto &i) {
        return i->getHostInfo().getAddress() == address;
      });

  if (server_it == hub->nodes.end()) {
    return result_t<ClientCtx>::Err("the server is not found by given address");
  }

  auto server = *server_it;

  std::vector<std::shared_ptr<Cnm::Node>> path{};
  path.emplace_back(node);
  path.emplace_back(hub);
  path.emplace_back(server);

  auto connection =
      std::make_shared<Connection>(hub->star->getNetworkSpeed(), path);

  auto ctx = ContextFactory::createServerContext<ServerContext>(
      connection, connection->getServerNode());
  server->serve(std::move(ctx));

  return result_t<ClientCtx>::Ok(
      ContextFactory::createClientContext<ClientContext>(
          connection, connection->getClientNode()));
}

std::vector<HostInfo> Communicator::getSpecificType(std::string_view type,
                                                    bool filter_unavailable) {
  std::unique_lock lock(hub->mutex);

  auto result =
      hub->nodes | std::views::filter([&](const auto &i) {
        return i->getType() == type && (!filter_unavailable && !i->isBusy());
      }) |
      std::views::transform([](const auto &i) { return i->getHostInfo(); });

  std::vector<HostInfo> r{};
  std::ranges::copy(result, std::back_inserter(r));

  return r;
}

}  // namespace Cnm::Star