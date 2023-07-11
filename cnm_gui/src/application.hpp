#ifndef HPP_CNM_GUI_APPLICATION_HPP
#define HPP_CNM_GUI_APPLICATION_HPP

#include <machines/machine.hpp>
#include <vector>

#include "helpers/menu.hpp"

// class Application;
//
// class MessageWindow {
//  public:
//   MessageWindow(Application&, std::string_view view, size_t id);
//   void render();
//
//   size_t id() const noexcept;
//
//   MessageWindow(const MessageWindow&) = default;
//   MessageWindow& operator=(const MessageWindow& o);
//
//  private:
//   Application& owner_;
//   std::string_view msg_;
//
//   size_t id_;
// };
//
// class Application {
//  public:
//   explicit Application();
//
//   ~Application();
//
//   void render();
//
//   bool shouldClose() const noexcept;
//
//   void removeMsgWindow(size_t id);
//
//  private:
//   helpers::Menu makeMenuBar();
//
//   void makeMsgWindow(std::string_view);
//
//   void handleAppClose();
//   void handleAppClear();
//
//   helpers::Menu menu_;
//
//   std::vector<MessageWindow> warnings_;
//
//   bool close_;
// };
//

class Application {
 public:
  explicit Application() = default;

  void render();

  bool shouldClose() const noexcept;
};

#endif  // HPP_CNM_GUI_APPLICATION_HPP
