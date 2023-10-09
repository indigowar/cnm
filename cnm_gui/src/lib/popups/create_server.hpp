#ifndef HPP_CNM_GUI_LIB_POPUPS_CREATE_SERVER_HPP
#define HPP_CNM_GUI_LIB_POPUPS_CREATE_SERVER_HPP

#include <functional>
#include <string>

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
  };

  CreateServerPopup() = default;

  void render(bool*);

  void onClose(std::function<void(Info&)> f) { on_close = std::move(f); }

  void onSave(std::function<void(Info&)> f) { on_save = std::move(f); }

 private:
  Info info;
  std::function<void(Info&)> on_close;
  std::function<void(Info&)> on_save;
};

}  // namespace Popup

#endif  // HPP_CNM_GUI_LIB_POPUPS_CREATE_SERVER_HPP