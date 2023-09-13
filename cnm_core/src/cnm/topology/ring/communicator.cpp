#include "communicator.hpp"

#include <memory>
#include <ranges>

#include "cnm/connection/client_ctx.hpp"
#include "cnm/connection/connection.hpp"
#include "cnm/topology/base/node.hpp"
#include "cnm/topology/ring/node.hpp"
#include "cnm/utils/result.hpp"
#include "spdlog/common.h"

namespace Cnm::Ring {

Communicator::Communicator(Ring* ring) : ring{ring} {}

std::vector<HostInfo> Communicator::getSpecificType(std::string_view type,
                                                    bool filter_unavailable) {
  std::unique_lock lock(ring->mutex);

  std::vector<HostInfo> result{};

  auto result_range =
      ring->nodes | std::views::values | std::views::filter([&](const auto& i) {
        return i->getType() == type && (!filter_unavailable && !i->isBusy());
      }) |
      std::views::transform([](const auto& i) { return i->getHostInfo(); });

  std::ranges::copy(result_range, std::back_inserter(result));

  return result;
}

void Communicator::disconnect(HostInfo host_info) {
  std::unique_lock lock(ring->mutex);

  if (!ring->nodes.contains(host_info.getAddress())) {
    return;
  }

  auto node = ring->nodes.at(host_info.getAddress());

  auto next = node->getNextNode();
  auto prev = node->getPreviousNode();

  if (next) next->setPreviousNode(nullptr);
  if (prev) prev->setNextNode(nullptr);

  ring->nodes.erase(host_info.getAddress());
}

void Communicator::setNode(std::shared_ptr<Cnm::Node> n) {
  this->node = std::move(n);
}

result_t<std::vector<std::shared_ptr<Node>>> Communicator::findShortestPath(
    const std::string& from, const std::string& to) {
  using ResultType = std::vector<std::shared_ptr<Node>>;

  if (!ring->nodes.contains(from)) {
    return result_t<ResultType>::Err(
        "node with given address for `from` is not exists in the topology");
  }

  if (!ring->nodes.contains(to)) {
    return result_t<ResultType>::Err(
        "node with given address for `to` is not exists in the topology");
  }

  auto start = ring->nodes.at(from);
  auto end = ring->nodes.at(to);

  ResultType next_path{};
  for (auto it = ++Iterator(start); (*it)->getHostInfo().getAddress() != from &&
                                    (*it)->getHostInfo().getAddress() != to;
       ++it) {
    next_path.emplace_back(*it);
  }

  if (next_path.size() >= ring->nodes.size() / 2 &&
      next_path.back().get() == end.get()) {
    return result_t<ResultType>::Ok(std::move(next_path));
  }

  ResultType back_path{};
  for (auto it = --Iterator(start); (*it)->getHostInfo().getAddress() != from &&
                                    (*it)->getHostInfo().getAddress() != to;
       ++it) {
    back_path.emplace_back(*it);
  }

  return result_t<ResultType>::Ok(
      std::move(next_path.size() > back_path.size() ? next_path : back_path));
}

result_t<ClientCtx> Communicator::makeConnection(std::string address) {
  auto path_result =
      findShortestPath(this->node->getHostInfo().getAddress(), address);

  auto path = path_result.unwrap();

  // TODO: replace this constant with member of Ring.
  constexpr size_t CONNECTION_SPEED = 100;

  Connection conn(CONNECTION_SPEED, path.begin(), path.end());

  // todo: create the server ctx for server
  std::ignore = std::async([this, &conn] {
    auto ctx = conn.createServerContext();
    if (ctx.isErr()) {
      spdlog::warn("Connection::createServerContext() - %s", ctx.unwrapErr());
      return;
    }

    ring->nodes.at(conn.getServerHostInfo().getAddress())
        ->serve(std::move(ctx.unwrap()));
  });

  return conn.createClientContext();
}

}  // namespace Cnm::Ring
