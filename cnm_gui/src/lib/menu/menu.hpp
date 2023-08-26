#ifndef HPP_CNM_GUI_LIB_MENU_MENU_NPP
#define HPP_CNM_GUI_LIB_MENU_MENU_NPP

#include <functional>
#include <string>
#include <vector>

namespace Menu {

// Item is only one button that submenu contains.
class Item final {
 public:
  Item(const std::string& name, std::function<void()> func);

  void render();

  [[nodiscard]] const std::string& getName() const noexcept;

  [[nodiscard]] const std::function<void()>& getAction() const noexcept;

 private:
  const std::string name;
  const std::function<void()> action;
};

// SubMenu is a button on menu, on click it opens a list of items.
class SubMenu final {
 public:
  SubMenu(const std::string& msg, std::vector<Item>&& items);

  void render();

  [[nodiscard]] const std::string& getName() const noexcept;

  [[nodiscard]] const std::vector<Item>& getItems() const noexcept;

 private:
  const std::string name;

  std::vector<Item> items;
};

// Menu is top-level menu which contains SubMenus.
class Menu final {
 public:
  explicit Menu(std::vector<SubMenu>&& items);

  void render();

 private:
  std::vector<SubMenu> items;
};

}  // namespace Menu

#endif  // HPP_CNM_GUI_LIB_MENU_MENU_NPP
