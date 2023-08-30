#include "ring_communicator.hpp"

#include <ranges>

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

result_t<ClientCtx> RingCommunicator::makeConnection(std::string_view address) {
  // TODO: Build a connection and return it's context.
}

}  // namespace Cnm