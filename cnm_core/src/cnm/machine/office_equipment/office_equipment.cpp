#include "office_equipment.hpp"

#include <spdlog/spdlog.h>

#include <utility>

namespace Cnm {

OfficeEquipment::OfficeEquipment(const OfficeEquipmentLogic& logic,
                                 HostInfo info,
                                 std::shared_ptr<Communicator> communicator)
    : Machine("office_equipment", 1, info, std::move(communicator)),
      logic(std::make_unique<OfficeEquipmentLogic>(logic)),
      is_accepting{},
      is_running{},
      tasks{} {}

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

  is_running = false;
}

size_t OfficeEquipment::getCurrentServingAmount() const noexcept {
  return busy.load() ? 1 : 0;
}

result_t<MessageBatch> OfficeEquipment::serve(MessageBatch request) {
  std::shared_ptr<std::promise<result_t<MessageBatch>>> result{};
  {
    auto lock = makeLock();
    if (!is_accepting) {
      return result_t<MessageBatch>::Err(
          "office equipment does not accept request right now");
    }
    result = std::make_shared<std::promise<result_t<MessageBatch>>>();
    tasks.emplace_back(request, result);
  }

  auto future = result->get_future();
  future.wait();
  return future.get();
}

void OfficeEquipment::threadFunction(const std::stop_token& stop_token) {
  auto current_logic = std::make_unique<OfficeEquipmentLogic>(*logic);
  logic->init();

  while (!stop_token.stop_requested()) {
    MessageBatch request;
    std::shared_ptr<std::promise<result_t<MessageBatch>>> promise{};

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
      auto [r, p] = tasks.front();
      request = r;
      promise = p;
    }

    busy.store(true);
    promise->set_value(current_logic->serve(request));
    busy.store(false);
  }
}

}  // namespace Cnm