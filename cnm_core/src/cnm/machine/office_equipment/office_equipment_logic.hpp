#ifndef HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_LOGIC_HPP
#define HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_LOGIC_HPP

#include <filesystem>

#include "cnm/connection/server_ctx.hpp"

namespace Cnm {

// OfficeEquipmentLogic - a stateless server of requests.
class OfficeEquipmentLogic {
 public:
  OfficeEquipmentLogic() = default;

  virtual void execute(ServerCtx&& ctx) { ctx->abort(); }
};

// ScannerOfficeEquipmentLogic - an OfficeEquipmentLogic that reads from a file.
class [[maybe_unused]] ScannerOfficeEquipmentLogic
    : public OfficeEquipmentLogic {
 public:
  explicit ScannerOfficeEquipmentLogic(std::filesystem::path base_dir);

  void execute(ServerCtx&&) override;

 private:
  // readFromFile - reads the content of a file into string
  static result_t<std::string> readFromFile(const std::filesystem::path&);

  void serveSimpleRequest(ServerCtx&& ctx, Message&& task);
  void serveComplexRequest(ServerCtx&& ctx, MessageBatch&& task);

  std::string handleFile(const std::string& filename);

  std::filesystem::path base_dir;
};

// PrinterOfficeEquipmentLogic - an OfficeEquipment that writes request in
// file.
class [[maybe_unused]] PrinterOfficeEquipmentLogic
    : public OfficeEquipmentLogic {
 public:
  explicit PrinterOfficeEquipmentLogic(std::filesystem::path base_dir);

  void execute(ServerCtx&&) override;

 private:
  static result_t<bool> writeIntoFile(const std::filesystem::path&,
                                      const std::string& content);

  const std::filesystem::path base_dir;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_LOGIC_HPP
