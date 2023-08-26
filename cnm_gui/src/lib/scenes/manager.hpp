#ifndef HPP_CNM_GUI_SCENES_LOADER_HPP
#define HPP_CNM_GUI_SCENES_LOADER_HPP

#include <map>
#include <memory>

#include "lib/scenes/exiter.hpp"
#include "lib/scenes/scene.hpp"
#include "lib/scenes/switcher.hpp"

namespace Scenes {

// Manager handles the scenes for application.
class Manager final {
 public:
  explicit Manager(Exiter* exiter);

  // add(std::shared_ptr<Scene>) - adds new scene to the pool of scenes for this
  // manager, after adding the scene can be set to be next by setNextScene call.
  void add(std::shared_ptr<Scene> new_scene);

  // update() - executes before creating the frame.
  void update();

  // render() - executes after update call and renders current scene.
  void render();

  // postRender() - handles the post render work, defined by Scene.
  void postRender();

  // cleanup() - cleans all Scenes.
  void cleanup();

  // setNextScene(std::string) - specifies the next scene by it's name.
  void setNextScene(std::string scene_name);

 private:
  class ManagerSwitcher final : public Scenes::Switcher {
   public:
    explicit ManagerSwitcher(Manager& m) : m{m} {}

    void switchScene(const std::string& scene) override {
      m.next_scene = scene;
    }

   private:
    Manager& m;
  };

  std::string current_scene;
  std::string next_scene;

  std::map<std::string, std::shared_ptr<Scene>> scenes;

  Exiter* exiter;
  ManagerSwitcher switcher;
};

}  // namespace Scenes

#endif  // HPP_CNM_GUI_SCENES_LOADER_HPP
