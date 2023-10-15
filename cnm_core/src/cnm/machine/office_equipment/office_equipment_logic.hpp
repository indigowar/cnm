#ifndef HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_LOGIC_HPP
#define HPP_CNM_CORE_MACHINE_OFFICE_EQUIPMENT_OFFICE_EQUIPMENT_LOGIC_HPP

#include <filesystem>

#include "cnm/connection/server_ctx.hpp"

// namespace Cnm {
//
// class OfficeEquipmentLogic {
//  public:
//   virtual ~OfficeEquipmentLogic() = default;
//
//   virtual void init() {}
//
//   virtual void execute(ServerCtx&&) {}
// };
//
//// ScannerLogic - the logic of the scanner.
//// For simplicity, it reads a file from the filesystem and returns the content
//// of the file.
// class [[maybe_unused]] ScannerLogic : public OfficeEquipmentLogic {
//  public:
//   void init() override;
//
//   void execute(ServerCtx&&) override;
//
//  private:
//   static std::string readFile(const std::string& name);
// };
//
//// PrinterLogic - the logic of the printer.
//// For simplicity, it writes into the filesystem.
// class [[maybe_unused]] PrinterLogic : public OfficeEquipmentLogic {
//  public:
//   void init() override;
//   void execute(ServerCtx&&) override;
// };
//
// }  // namespace Cnm

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

// PrinterOfficeEquipmentLogic - an OfficeEquipment that writes request in file.
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
