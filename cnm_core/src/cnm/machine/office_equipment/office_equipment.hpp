#ifndef HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_HPP
#define HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_HPP

#include <atomic>
#include <condition_variable>
#include <deque>
#include <future>
#include <memory>
#include <thread>
#include <tuple>

#include "cnm/core/message.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/machine/office_equipment/office_equipment_logic.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

// OfficeEquipment is a machine in the network that can serve one request at the
// time.
class OfficeEquipment final : public Machine {
 public:
  OfficeEquipment(const OfficeEquipmentLogic&, HostInfo,
                  std::unique_ptr<Communicator>);

  ~OfficeEquipment() override;

  void start() override;

  void stop() override;

  void invoke() override;

  void freeze() override;

  Object::Status getStatus() const noexcept override;

  size_t getCurrentServingAmount() const noexcept override;

  void serve(ServerCtx&& ctx) override;

  static constexpr std::string_view Type = "office_equipment";

 private:
  void threadFunction(const std::stop_token& stop_token);

  // is_accepting is true when the object can accept new requests.
  bool is_accepting;
  // is_running is true when the object executes(the thread is alive).
  bool is_running;

  std::atomic_bool busy;

  std::deque<ServerCtx> tasks;

  std::condition_variable cond_var;
  std::unique_ptr<std::jthread> thread;

  std::unique_ptr<OfficeEquipmentLogic> logic;

  Object::Status status;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_HPP
