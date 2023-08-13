#include "office_equipment.hpp"

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
  // start the execution of the thread and start to accept the requests.
  auto lock = makeLock();
  is_running = true;
  is_accepting = true;
  thread = std::make_unique<std::jthread>(
      [this](const std::stop_token& st) { threadFunction(st); });
}

void OfficeEquipment::stop() {
  // kill the thread and stop accepting the requests.
  auto lock = makeLock();
  thread->request_stop();
  tasks.clear();
  thread.reset();
}

void OfficeEquipment::invoke() {
  // start the execution  of sleeping thread.
  {
    auto lock = makeLock();
    is_accepting = true;
    is_running = true;
  }
  cond_var.notify_one();
}

void OfficeEquipment::freeze() {
  // stop the execution of the thread, but without killing it.
  // and still accepts the requests.
  {
    auto lock = makeLock();
    is_running = false;
  }
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
