#include "personal_computer.hpp"

#include <spdlog/spdlog.h>

namespace Cnm {

PersonalComputer::PersonalComputer(Cnm::PersonalComputerLogic&& logic,
                                   Cnm::HostInfo host_info,
                                   std::unique_ptr<Communicator>&& communicator)
    : Machine(PersonalComputer::Type, 0, host_info, std::move(communicator)),
      logic(std::move(logic)),
      thread{},
      continue_execution{} {}

PersonalComputer::~PersonalComputer() { stop(); }

size_t PersonalComputer::getCurrentServingAmount() const noexcept { return 0; }

void PersonalComputer::serve(ServerCtx&& ctx) {
  spdlog::warn("PersonalComputer::serve(): was called.");
  //  return result_t<MessageBatch>::Err("pc can not serve.");
}

void PersonalComputer::start() {
  auto lock = makeLock();

  if (thread) {
    spdlog::warn("PersonalComputer::start(): the thread is already created.");
    return;
  }

  if (continue_execution) {
    spdlog::warn("PersonalComputer::start(): already running.");
    return;
  }

  continue_execution = true;

  thread = std::make_unique<std::jthread>(
      [this](std::stop_token t) { threadRunner(std::move(t)); });
}

void PersonalComputer::stop() {
  auto lock = makeLock();

  if (!thread) {
    spdlog::warn("PersonalComputer::stop(): PC has no running thread.");
    return;
  }

  if (!continue_execution) {
    spdlog::info("PersonalComputer::stop(): stopping frozen thread.");
  }

  thread->request_stop();
  if (thread->joinable()) {
    thread->join();
  }
}

void PersonalComputer::invoke() {
  {
    auto lock = makeLock();

    if (!thread) {
      spdlog::warn("PersonalComputer::invoke(): PC has no running thread.");
      return;
    }

    if (continue_execution) {
      spdlog::warn("PersonalComputer::invoke(): PC has active thread.");
      return;
    }

    continue_execution = true;
  }
  cond_var.notify_one();
}

void PersonalComputer::freeze() {
  auto lock = makeLock();
  continue_execution = false;
}

void PersonalComputer::threadRunner(std::stop_token stop_token) {
  // With creation of thread we init the logic for the runner.
  auto running_logic = logic;
  running_logic.init();

  while (!stop_token.stop_requested()) {
    auto lock = makeLock();
    cond_var.wait(lock, [this, &stop_token] {
      return continue_execution || stop_token.stop_requested();
    });

    if (continue_execution) {
      running_logic.execute();
    }
  }
}

}  // namespace Cnm
