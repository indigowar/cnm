#include "office_equipment.hpp"

#include <spdlog/spdlog.h>

namespace Cnm {

OfficeEquipment::OfficeEquipment(Cnm::OfficeEquipmentLogic &&logic,
                                 Cnm::HostInfo host_info,
                                 std::unique_ptr<Communicator> &&c)
    : Machine(OfficeEquipment::Type, 1, std::move(host_info), std::move(c)),
      runner(nullptr),
      is_active{},
      logic{logic} {}

OfficeEquipment::~OfficeEquipment() { stop(); }

size_t OfficeEquipment::getCurrentServingAmount() const noexcept {
  auto lock = makeLock();
  if (!runner) {
    return 0;
  }
  return runner->getNumBusyWorkers();
}

void OfficeEquipment::start() {
  auto lock = makeLock();

  if (runner) {
    spdlog::warn("OfficeEquipment::start() - the runner exists.");
    return;
  }

  runner = std::make_unique<Utils::ThreadPool>(getServingLimit());
  runner->startThreads();
  is_active = true;
}

void OfficeEquipment::stop() {
  auto lock = makeLock();

  if (!runner) {
    spdlog::warn("OfficeEquipment::stop() - already stopped.");
    return;
  }

  is_active = false;
  runner.reset(nullptr);
}

void OfficeEquipment::invoke() {
  auto lock = makeLock();

  if (!runner) {
    spdlog::warn(
        "OfficeEquipment::invoke() - the runner does not exist(stopped).");
    return;
  }

  if (is_active) {
    spdlog::warn("OfficeEquipment::invoke() - currently already running.");
    return;
  }
  is_active = true;
}

void OfficeEquipment::freeze() {
  auto lock = makeLock();

  if (!runner) {
    spdlog::warn(
        "OfficeEquipment::freeze() - the runner does not exist(stopped).");
    return;
  }

  if (!is_active) {
    spdlog::warn("OfficeEquipment::freeze() - currently frozen.");
    return;
  }

  is_active = false;
}

Object::Status OfficeEquipment::getStatus() const noexcept {
  auto lock = makeLock();
  return !runner ? Dead : (is_active ? Running : Freezed);
}

void OfficeEquipment::serve(ServerCtx &&ctx) {
  auto lock = makeLock();

  if (is_active) {
    addRequest(std::move(ctx));
    return;
  }

  spdlog::warn(
      "OfficeEquipment::serve called when unable to accept the request.");
  ctx->abort();
}

void OfficeEquipment::addRequest(Cnm::ServerCtx &&ctx) {
  auto task = [this, &ctx] { logic.execute(std::move(ctx)); };
  runner->enqueue(task);
}

}  // namespace Cnm