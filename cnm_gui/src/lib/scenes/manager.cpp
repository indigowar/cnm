#include "manager.hpp"

#include "spdlog/spdlog.h"

namespace Scenes {

Manager::Manager(Exiter* exiter)
    : switcher(*this),
      exiter(exiter),
      next_scene{},
      current_scene{},
      scenes() {}

void Manager::add(std::shared_ptr<Scene> new_scene) {
  if (scenes.contains(new_scene->getName())) {
    spdlog::warn("Manager::add: tried to add already existing scene");
    return;
  }
  new_scene->setExiter(exiter);
  new_scene->setSwitcher(&switcher);

  scenes.emplace(new_scene->getName(), new_scene);
}

void Manager::update() {
  if (current_scene == next_scene) {
    scenes.at(current_scene)->update();
    return;
  }

  if (scenes.contains(current_scene)) {
    scenes.at(current_scene)->freeze();
  }
  current_scene = next_scene;

  auto current = scenes.at(current_scene);
  if (current->hasStarted()) {
    current->invoke();
  } else {
    current->callStart();
  }
}

void Manager::render() { scenes.at(current_scene)->render(); }

void Manager::postRender() { scenes.at(current_scene)->postRender(); }

void Manager::cleanup() {
  for (auto& [name, scene] : scenes) {
    scene->cleanup();
  }
}

void Manager::setNextScene(std::string scene_name) {
  switcher.switchScene(scene_name);
}

}  // namespace Scenes
