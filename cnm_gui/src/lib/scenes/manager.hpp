#ifndef HPP_CNM_GUI_SCENES_LOADER_HPP
#define HPP_CNM_GUI_SCENES_LOADER_HPP

#ifdef _DEBUG
#include <spdlog/spdlog.h>
#endif

#include <map>
#include <memory>

#include "lib/scenes/scene.hpp"

namespace scene {

class Manager final {
  class SceneSwitcher final : public ISceneSwitcher {
   public:
    explicit SceneSwitcher(Manager& m) : m{m} {}

    void switch_scene(std::string_view next_scene_name) {
      m.next_scene = next_scene_name;
    }

   private:
    Manager& m;
  };

 public:
  explicit Manager(IExitter* exitter)
      : scene_switcher(*this),
        exitter(exitter),
        next_scene{},
        active_scene{},
        scenes() {}

  void add(std::shared_ptr<Scene> new_scene) {
    if (scenes.contains(new_scene->get_name())) {
#ifdef _DEBUG
      spdlog::warn("tried to add already existing scene");
#endif
      return;
    }
    new_scene->set_exitter(exitter);
    new_scene->set_switcher(&scene_switcher);
    scenes[new_scene->get_name()] = new_scene;
  }

  void update() {
    if (active_scene == next_scene) {
      scenes.at(active_scene)->update();
      return;
    }

    if (scenes.contains(active_scene)) scenes.at(active_scene)->froze();
    active_scene = next_scene;

    auto current = scenes.at(active_scene);

    if (current->has_started()) {
      current->invoke();
    } else {
      current->call_start();
    }
  }

  void render() { scenes.at(active_scene)->render(); }

  void post_render() { scenes.at(active_scene)->post_render(); }

  void cleanup() {
    for (const auto& [name, scene] : scenes) {
      scene->cleanup();
    }
  }

  void set_next_scene(std::string_view scene_name) {
    scene_switcher.switch_scene(scene_name);
  }

 private:
  SceneSwitcher scene_switcher;
  IExitter* exitter;

  std::string_view next_scene;
  std::string_view active_scene;

  std::map<std::string_view, std::shared_ptr<Scene>> scenes;
};

}  // namespace scene

#endif  // HPP_CNM_GUI_SCENES_LOADER_HPP
