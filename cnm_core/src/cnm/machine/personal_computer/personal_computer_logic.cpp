#include "personal_computer_logic.hpp"

#include <spdlog/spdlog.h>

#include <random>

template <typename Numeric>
  requires std::integral<Numeric>
Numeric generateRandomNumber(Numeric min, Numeric max) {
  if (min >= max) {
    return min;
  }
  static std::random_device rd;
  static std::default_random_engine el(rd());
  static std::uniform_int_distribution<Numeric> distribution(min, max);
  return distribution(el);
}

std::string generateRandomString(size_t length) {
  const std::string CHARACTERS =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);
  std::string random_string;
  for (size_t i = 0; i < length; ++i) {
    random_string += CHARACTERS[distribution(generator)];
  }
  return random_string;
}

namespace Cnm {

void RandomPersonalComputerLogic::init() {}

void RandomPersonalComputerLogic::execute(
    std::unique_ptr<Communicator>& communicator) {
  auto machine_type = generateRandomNumber<size_t>(0, 1);
  std::vector<HostInfo> machines = machine_type == 0
                                       ? communicator->getServers()
                                       : communicator->getOfficeEquipments();
  if (machines.empty()) {
    return;
  }
  auto machine_id = generateRandomNumber<size_t>(0, machines.size() - 1);
  auto machine_info = machines.at(machine_id);
  auto connection_result =
      communicator->makeConnection(machine_info.getAddress());
  if (connection_result.isErr()) {
    spdlog::warn("RPCL::execute() - Failed to connect: %s",
                 connection_result.unwrapErr());
    return;
  }
  auto ctx = connection_result.unwrap();
  auto msg = generateRandomString(generateRandomNumber<size_t>(1, 1000));
  if (ctx->waitUntilAccepted().isErr()) {
    spdlog::warn("RPCL::execute() - Failed to be accepted.");
    return;
  }
  ctx->sendRequest(MessageBatch(std::move(msg)));
  auto response = ctx->waitAndGetResponse();
  if (response.isErr()) {
    spdlog::warn("RPCL::execute() - Failed to get response.");
    return;
  }
}

}  // namespace Cnm
