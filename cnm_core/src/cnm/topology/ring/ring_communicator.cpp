#include "ring_communicator.hpp"

#include <memory>
#include <ranges>

#include "cnm/connection/client_ctx.hpp"
#include "cnm/connection/connection.hpp"
#include "cnm/topology/base/node.hpp"
#include "cnm/topology/ring/ring_node.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

RingCommunicator::RingCommunicator(Ring* ring) : ring{ring} {}

std::vector<HostInfo> RingCommunicator::getSpecificType(
    std::string_view type, bool filter_unavailable) {
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

void RingCommunicator::disconnect(HostInfo host_info) {
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

void RingCommunicator::setNode(std::shared_ptr<Node> n) {
  this->node = std::move(n);
}

result_t<std::vector<std::shared_ptr<RingNode>>>
RingCommunicator::findShortestPath(const std::string& from,
                                   const std::string& to) {
  using ResultType = std::vector<std::shared_ptr<RingNode>>;

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
  for (auto it = ++RingIterator(start);
       (*it)->getHostInfo().getAddress() != from &&
       (*it)->getHostInfo().getAddress() != to;
       ++it) {
    next_path.emplace_back(*it);
  }

  if (next_path.size() >= ring->nodes.size() / 2 &&
      next_path.back().get() == end.get()) {
    return result_t<ResultType>::Ok(std::move(next_path));
  }

  ResultType back_path{};
  for (auto it = --RingIterator(start);
       (*it)->getHostInfo().getAddress() != from &&
       (*it)->getHostInfo().getAddress() != to;
       ++it) {
    back_path.emplace_back(*it);
  }

  return result_t<ResultType>::Ok(
      std::move(next_path.size() > back_path.size() ? next_path : back_path));
}

result_t<ClientCtx> RingCommunicator::makeConnection(std::string address) {
  // TODO: Build a connection and return it's context.
}

}  // namespace Cnm
