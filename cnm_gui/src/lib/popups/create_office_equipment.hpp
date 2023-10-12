#ifndef HPP_CNM_GUI_LIB_POPUPS_CREATE_OFFICE_EQUIPMENT_HPP
#define HPP_CNM_GUI_LIB_POPUPS_CREATE_OFFICE_EQUIPMENT_HPP

#include <functional>
#include <string>

namespace Popup {

class CreateOfficeEquipmentPopup final {
 public:
  enum class OfficeEquipmentType { Scanner, Printer };

  struct Info final {
    struct {
      int f;
      int s;
      int t;
      int fo;
    } ip;

    std::string name;

    OfficeEquipmentType type;
  };

  CreateOfficeEquipmentPopup() = default;

  void render(bool*);

  void onClose(std::function<void(Info&)> f) { on_close = std::move(f); }

  void onSave(std::function<void(Info&)> f) { on_save = std::move(f); }

 private:
  std::function<void(Info&)> on_close;
  std::function<void(Info&)> on_save;

  Info info;
};

}  // namespace Popup

#endif  // HPP_CNM_GUI_LIB_POPUPS_CREATE_OFFICE_EQUIPMENT_HPP