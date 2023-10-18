#ifndef HPP_CNM_GUI_LIB_POPUPS_CREATE_PERSONAL_COMPUTER_HPP
#define HPP_CNM_GUI_LIB_POPUPS_CREATE_PERSONAL_COMPUTER_HPP

#include "lib/render/list_chooser.hpp"
#include "lib/render/readers.hpp"

namespace Popup {

class CreatePC {
 public:
  enum class Type { NotSpecified, Randomized, Controlled };

  struct Info {
    struct {
      int f;
      int s;
      int t;
      int fo;
    } ip;

    std::string name;
    Type type;
  };

  CreatePC();

  void render(bool*);

  void onSave(std::function<void(Info&)> f) { on_save = std::move(f); }

 private:
  Info makeInfo();
  void reset();
  static bool validate(const Info&);

  StringReader<256> name_reader;
  NumberReader<uint8_t, 10, 3> first_ip;
  NumberReader<uint8_t, 10, 3> second_ip;
  NumberReader<uint8_t, 10, 3> third_ip;
  NumberReader<uint8_t, 10, 3> fourth_ip;
  ListChooser<2> type_chooser;

  std::function<void(Info&)> on_save;
};

}  // namespace Popup

#endif  // HPP_CNM_GUI_LIB_POPUPS_CREATE_PERSONAL_COMPUTER_HPP
