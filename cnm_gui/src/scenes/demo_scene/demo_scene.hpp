#ifndef HPP_CNM_GUI_SCENES_DEMO_SCENE_HPP
#define HPP_CNM_GUI_SCENES_DEMO_SCENE_HPP

#include <memory>

#include "lib/scenes/exiter.hpp"
#include "lib/scenes/scene.hpp"
#include "lib/scenes/switcher.hpp"

class DemoScene final : public Scenes::Scene {
 public:
  DemoScene(Scenes::Switcher* = nullptr, Scenes::Exiter* = nullptr);
  void start() override;

  void update() override;

  void render() override;

  void postRender() override;

  void cleanup() override;

  void freeze() override;

  void invoke() override;
};

#endif  // HPP_CNM_GUI_SCENES_DEMO_SCENE_HPP
