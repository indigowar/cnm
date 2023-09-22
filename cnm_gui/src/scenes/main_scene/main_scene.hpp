#ifndef HPP_CNM_GUI_SCENES_MAIN_SCENE_HPP
#define HPP_CNM_GUI_SCENES_MAIN_SCENE_HPP

#include <cnm/topology/star/star.hpp>
#include <map>
#include <memory>

#include "lib/menu/menu.hpp"
#include "lib/scenes/exiter.hpp"
#include "lib/scenes/scene.hpp"
#include "lib/scenes/switcher.hpp"

class MainScene final : public Scenes::Scene {
 public:
  MainScene(Scenes::Switcher* switcher = nullptr,
            Scenes::Exiter* exitter = nullptr);
  void start() override;

  void update() override;

  void render() override;

  void postRender() override;

  void cleanup() override;

  void freeze() override;

  void invoke() override;

 private:
  void renderEditor();

  Menu::Menu makeMenuBar();

  std::unique_ptr<Menu::Menu> menu;

  std::unique_ptr<Cnm::Star::Star> topology;

  bool open_server_creation_window;

  void showServerCreationWindow(bool* open);
};

#endif  // HPP_CNM_GUI_SCENES_MAIN_SCENE_HPP
