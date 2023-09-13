#ifndef HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_LOGIC_HPP
#define HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_LOGIC_HPP

namespace Cnm {

class OfficeEquipmentLogic {
 public:
  virtual ~OfficeEquipmentLogic() = default;

  virtual void init() {}

  virtual void execute(ServerCtx&&) {}
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_LOGIC_HPP
