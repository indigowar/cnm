#ifndef HPP_CNM_GUI_LIB_POPUPS_CREATE_OFFICE_EQUIPMENT_HPP
#define HPP_CNM_GUI_LIB_POPUPS_CREATE_OFFICE_EQUIPMENT_HPP

#include <functional>
#include <string>

#include "lib/render/list_chooser.hpp"
#include "lib/render/readers.hpp"

namespace Popup {

class CreateOfficeEquipmentPopup final {
 public:
  enum class Type { NotSelected, Scanner, Printer };

  struct Info final {
    struct {
      int f;
      int s;
      int t;
      int fo;
    } ip;

    std::string name;

    std::string base_dir;

    Type type;
  };

  CreateOfficeEquipmentPopup() = default;

  void render(bool*);

  void onSave(std::function<void(Info&)> f) { on_save = std::move(f); }

 private:
  Info makeInfo();
  bool validate(Info);
  void reset();

  StringReader<256> name_reader{};
  StringReader<1024> base_dir{};

  NumberReader<uint8_t, 10, 3> first_reader{};
  NumberReader<uint8_t, 10, 3> second_reader{};
  NumberReader<uint8_t, 10, 3> third_reader{};
  NumberReader<uint8_t, 10, 3> fourth_reader{};

  ListChooser<2> type_chooser{{"Printer", "Scanner"}};

  std::function<void(Info&)> on_save;
};

}  // namespace Popup

#endif  // HPP_CNM_GUI_LIB_POPUPS_CREATE_OFFICE_EQUIPMENT_HPP