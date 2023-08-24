#ifndef HPP_CNM_GUI_SCENES_MAIN_SCENE_HPP
#define HPP_CNM_GUI_SCENES_MAIN_SCENE_HPP

#include <imgui_internal.h>

#include <memory>

#include "helpers/menu.hpp"
#include "lib/scenes/scene.hpp"

class MainScene final : public scene::Scene {
 public:
  MainScene(scene::ISceneSwitcher* switcher = nullptr,
            scene::IExitter* exitter = nullptr);
  void start() override;

  void update() override;

  void render() override;

  void post_render() override;

  void cleanup() override;

  void froze() override;

  void invoke() override;

 private:
  void render_editor();

  void render_props();

  helpers::Menu makeMenuBar();

  std::unique_ptr<helpers::Menu> menu;
};

#endif  // HPP_CNM_GUI_SCENES_MAIN_SCENE_HPP
