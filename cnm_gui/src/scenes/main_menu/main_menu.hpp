#ifndef HPP_CNM_GUI_SCENES_MAIN_MENU_HPP
#define HPP_CNM_GUI_SCENES_MAIN_MENU_HPP

#include "lib/scenes/scene.hpp"

class MainMenu final : public Scenes::Scene {
 public:
  MainMenu();

  void start() override {}

  void update() override {}

  void render() override;

  void postRender() override {}

  void cleanup() override {}

  void freeze() override {}

  void invoke() override {}
};

#endif  // HPP_CNM_GUI_SCENES_MAIN_MENU_HPP
