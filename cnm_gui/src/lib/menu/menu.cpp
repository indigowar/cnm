#include "menu.hpp"

#include <imgui.h>

#include <algorithm>

namespace Menu {

Item::Item(const std::string& name, std::function<void()> func)
    : name{name}, action{std::move(func)} {}

void Item::render() {
  if (ImGui::MenuItem(name.c_str())) {
    action();
  }
}

const std::string& Item::getName() const noexcept { return name; }

const std::function<void()>& Item::getAction() const noexcept { return action; }

SubMenu::SubMenu(const std::string& msg, std::vector<Item>&& items)
    : name{msg}, items{std::move(items)} {}

void SubMenu::render() {
  if (ImGui::BeginMenu(name.c_str())) {
    std::for_each(items.begin(), items.end(), [](auto& i) { i.render(); });
    ImGui::EndMenu();
  }
}

[[nodiscard]] const std::string& SubMenu::getName() const noexcept {
  return name;
}

[[nodiscard]] const std::vector<Item>& SubMenu::getItems() const noexcept {
  return items;
}

Menu::Menu(std::vector<SubMenu>&& items) : items{std::move(items)} {}

void Menu::render() {
  if (ImGui::BeginMainMenuBar()) {
    std::for_each(items.begin(), items.end(), [](auto& i) { i.render(); });
    ImGui::EndMainMenuBar();
  }
}

}  // namespace Menu
