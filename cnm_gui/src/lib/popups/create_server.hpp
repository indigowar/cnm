#ifndef HPP_CNM_GUI_LIB_POPUPS_CREATE_SERVER_HPP
#define HPP_CNM_GUI_LIB_POPUPS_CREATE_SERVER_HPP

#include <functional>
#include <string>

#include "lib/render/list_chooser.hpp"
#include "lib/render/readers.hpp"

namespace Popup {

class CreateServerPopup final {
 public:
  enum class ServerType {
    FileServer,
  };

  struct Info final {
    struct {
      int f;
      int s;
      int t;
      int fo;
    } ip;

    std::string name;

    ServerType type;

    std::optional<std::map<std::string, std::string>> files;

    size_t limit;
  };

  CreateServerPopup();

  void render(bool*);

  void onSave(std::function<void(Info&)> f) { on_save = std::move(f); }

 private:
  Info makeInfo();
  void reset();

  bool validate(Info);

  StringReader<256> name_reader;

  NumberReader<uint8_t, 10, 3> first_ip;
  NumberReader<uint8_t, 10, 3> second_ip;
  NumberReader<uint8_t, 10, 3> third_ip;
  NumberReader<uint8_t, 10, 3> fourth_ip;

  NumberReader<size_t, 10, 100> limit_reader;

  ListChooser<2> type_chooser;

  MapReader file_reader;

  std::function<void(Info&)> on_save;
};

}  // namespace Popup

#endif  // HPP_CNM_GUI_LIB_POPUPS_CREATE_SERVER_HPP