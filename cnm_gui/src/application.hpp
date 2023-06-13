#ifndef HPP_CNM_GUI_APPLICATION_HPP
#define HPP_CNM_GUI_APPLICATION_HPP

#include <machines/machine.hpp>

#include "helpers/menu.hpp"

class Application {
 public:
  explicit Application();

  ~Application();

  void render();

  bool shouldClose() const noexcept;

 private:
  helpers::Menu makeMenuBar();

  void handleAppClose();
  void handleAppClear();

  helpers::Menu menu_;

  bool close_;
};

#endif  // HPP_CNM_GUI_APPLICATION_HPP
