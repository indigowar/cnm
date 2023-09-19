#include "office_equipment.hpp"

#include <spdlog/spdlog.h>

#include <utility>

namespace Cnm {

OfficeEquipment::OfficeEquipment(const OfficeEquipmentLogic& logic,
                                 HostInfo info,
                                 std::unique_ptr<Communicator> communicator)
    : Machine(OfficeEquipment::Type, 1, info, std::move(communicator)),
      logic(std::make_unique<OfficeEquipmentLogic>(logic)),
      is_accepting{},
      is_running{},
      tasks{},
      status{NotInitialized} {}

OfficeEquipment::~OfficeEquipment() { stop(); }

void OfficeEquipment::start() {
  auto lock = makeLock();

  if (is_running) {
    spdlog::warn("OfficeEquipment::start(): called when is running already.");
    return;
  }

  if (thread) {
    spdlog::warn(
        "OfficeEquipment::start(): called when the thread is initialized.");
    return;
  }

  is_accepting = true;
  is_running = true;

  status = Running;

  thread = std::make_unique<std::jthread>(
      [this](const std::stop_token& st) { threadFunction(st); });
}

void OfficeEquipment::stop() {
  auto lock = makeLock();

  if (!thread) {
    spdlog::warn("OfficeEquipment::stop(): called when the thread is dead.");
    return;
  }

  is_running = false;
  is_accepting = false;
  status = Dead;

  thread->request_stop();
  thread.reset();
}

void OfficeEquipment::invoke() {
  {
    auto lock = makeLock();

    if (!thread) {
      spdlog::warn("OfficeEquipment::invoke(): called on the dead thread.");
      return;
    }

    if (is_running) {
      spdlog::warn("OfficeEquipment::invoke(): called on the running thread.");
      return;
    }

    is_running = true;
  }

  status = Running;

  cond_var.notify_one();
}

void OfficeEquipment::freeze() {
  auto lock = makeLock();

  if (!thread) {
    spdlog::warn("OfficeEquipment::freeze(): called on the dead thread.");
    return;
  }

  if (!is_running) {
    spdlog::warn("OfficeEquipment::freeze(): called on the frozen thread.");
    return;
  }

  status = Freezed;

  is_running = false;
}

size_t OfficeEquipment::getCurrentServingAmount() const noexcept {
  return busy.load() ? 1 : 0;
}

void OfficeEquipment::serve(ServerCtx&& ctx) {
  auto lock = makeLock();

  if (!is_accepting) {
    spdlog::warn("office equipment does not accept request right now");
    // ctx->abort();
    return;
  }

  tasks.emplace_back(std::move(ctx));
}

void OfficeEquipment::threadFunction(const std::stop_token& stop_token) {
  auto current_logic = std::make_unique<OfficeEquipmentLogic>(*logic);
  logic->init();

  while (!stop_token.stop_requested()) {
    ServerCtx ctx{};

    {
      auto lock = makeLock();

      if (tasks.empty() || !is_running) {
        if (stop_token.stop_requested()) {
          return;
        }

        cond_var.wait(lock, [this, &stop_token] {
          return (is_running && !tasks.empty()) || stop_token.stop_requested();
        });
      }

      if (stop_token.stop_requested()) {
        return;
      }
      ctx = std::move(tasks.front());
    }

    busy.store(true);
    current_logic->execute(std::move(ctx));
    busy.store(false);
  }
}

}  // namespace Cnm
