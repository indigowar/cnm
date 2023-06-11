#ifndef HPP_CNM_GUI_HELPERS_MENU_HPP
#define HPP_CNM_GUI_HELPERS_MENU_HPP

#include <imgui.h>

#include <functional>
#include <string_view>
#include <vector>

namespace helpers {

class MenuField final {
 public:
  MenuField(std::string_view message, std::function<void()> action)
      : message_{message}, action_{std::move(action)} {}

  void render() {
    if (ImGui::MenuItem(message_.data())) {
      action_();
    }
  }

  std::string_view message() const noexcept { return message_; }
  std::function<void()> action() const noexcept { return action_; }

 private:
  std::string_view message_;
  std::function<void()> action_;
};

class MenuItem final {
 public:
  MenuItem(std::string_view message, std::vector<MenuField> items)
      : message_{message}, items_{std::move(items)} {}

  void render() {
    if (ImGui::BeginMenu(message_.data())) {
      for (auto& item : items_) item.render();
      ImGui::EndMenu();
    }
  }

  std::string_view message() const noexcept { return message_; }

  const std::vector<MenuField>& items() const noexcept { return items_; }

 private:
  std::string_view message_;
  std::vector<MenuField> items_;
};

class Menu final {
 public:
  explicit Menu(std::vector<MenuItem>&& items) : items_{items} {}

  void render() {
    if (ImGui::BeginMainMenuBar()) {
      for (auto& i : items_) i.render();
    }
    ImGui::EndMainMenuBar();
  }

 private:
  std::vector<MenuItem> items_;
};

}  // namespace helpers

#endif  // HPP_CNM_GUI_HELPERS_MENU_HPP
