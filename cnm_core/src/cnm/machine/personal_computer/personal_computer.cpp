#include "personal_computer.hpp"

#include <spdlog/spdlog.h>

namespace Cnm {

PersonalComputer::PersonalComputer(
    std::unique_ptr<PersonalComputerLogic>&& logic, HostInfo,
    std::unique_ptr<Communicator>&&)
    : Machine(PersonalComputer::Type, 0, host_info, std::move(communicator)),
      logic(std::move(logic)),
      thread{},
      continue_execution{},
      status{NotInitialized} {}

PersonalComputer::~PersonalComputer() { stop(); }

size_t PersonalComputer::getCurrentServingAmount() const noexcept { return 0; }

void PersonalComputer::serve(ServerCtx&& ctx) {
  spdlog::warn("PersonalComputer::serve(): was called.");
  ctx->abort();
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

  status = Running;
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

  continue_execution = false;
  thread->request_stop();

  if (thread->joinable()) {
    thread->detach();
    thread.reset(nullptr);
  }

  status = Dead;
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
  status = Running;
  cond_var.notify_one();
}

void PersonalComputer::freeze() {
  auto lock = makeLock();
  continue_execution = false;
  status = Freezed;
}

void PersonalComputer::threadRunner(std::stop_token stop_token) {
  logic->reset();
  logic->init();
  while (!stop_token.stop_requested()) {
    auto lock = makeLock();
    cond_var.wait(lock, [this, &stop_token] {
      return continue_execution || stop_token.stop_requested();
    });
    if (stop_token.stop_requested()) {
      return;
    }
    if (continue_execution) {
      logic->execute(communicator);
    }
  }
}

Object::Status PersonalComputer::getStatus() const noexcept { return status; }

}  // namespace Cnm
