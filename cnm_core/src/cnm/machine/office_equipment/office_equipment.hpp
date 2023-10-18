#ifndef HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_HPP
#define HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_HPP

#include "cnm/machine/machine.hpp"
#include "cnm/machine/office_equipment/office_equipment_logic.hpp"
#include "cnm/utils/thread_pool.hpp"

namespace Cnm {

// OfficeEquipment - an office equipment in the network.
// Can serve one request at a time.
class OfficeEquipment final : public Machine {
 public:
  OfficeEquipment(std::unique_ptr<OfficeEquipmentLogic> &&logic,
                  HostInfo host_info, std::unique_ptr<Communicator> &&);

  ~OfficeEquipment() override;

  size_t getCurrentServingAmount() const noexcept override;

  void start() override;

  void stop() override;

  void invoke() override;

  void freeze() override;

  Object::Status getStatus() const noexcept override;

  void serve(ServerCtx &&) override;

  static constexpr std::string_view Type = "office_equipment";

 private:
  void addRequest(ServerCtx &&);

  std::unique_ptr<Utils::ThreadPool> runner;

  bool is_active;

  std::unique_ptr<OfficeEquipmentLogic> logic;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_HPP
