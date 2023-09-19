#ifndef HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_LOGIC_HPP
#define HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_LOGIC_HPP

#include "cnm/connection/server_ctx.hpp"

namespace Cnm {

class OfficeEquipmentLogic {
 public:
  virtual ~OfficeEquipmentLogic() = default;

  virtual void init() {}

  virtual void execute(ServerCtx&&) {}
};

// ScannerLogic - the logic of the scanner.
// For simplicity, it reads a file from the filesystem and returns the content
// of the file.
class [[maybe_unused]] ScannerLogic : public OfficeEquipmentLogic {
 public:
  void init() override;

  void execute(ServerCtx&&) override;

 private:
  static std::string readFile(const std::string& name);
};

// PrinterLogic - the logic of the printer.
// For simplicity, it writes into the filesystem.
class [[maybe_unused]] PrinterLogic : public OfficeEquipmentLogic {
 public:
  void init() override;
  void execute(ServerCtx&&) override;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_LOGIC_HPP
