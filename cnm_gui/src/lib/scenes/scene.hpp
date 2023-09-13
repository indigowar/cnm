#ifndef HPP_CNM_GUI_SCENES_SCENE_HPP
#define HPP_CNM_GUI_SCENES_SCENE_HPP

#include "lib/scenes/exiter.hpp"
#include "lib/scenes/switcher.hpp"

namespace Scenes {

// Scene is an object that contains state and logic for specific "sub-program",
// that can be loaded into main application.
class Scene {
 public:
  Scene(std::string name, Switcher* switcher = nullptr,
        Exiter* exiter = nullptr)
      : name{name}, switcher{switcher}, exiter{exiter}, started{} {}

  virtual ~Scene() = default;

  // getName() - returns the name of this scene
  std::string getName() const noexcept { return name; }

  // hasStarted() - returns true, if the start() method has been called
  bool hasStarted() const noexcept { return started; }

  void callStart() {
    start();
    started = true;
  }

  // start() - executes on the first invoke of the scene(one-time).
  virtual void start() {}

  // update() - executes every update before rendering.
  virtual void update() {}

  // render() - executes to render the scene.
  virtual void render() {}

  // postRender() - executes after rendering the frame.
  virtual void postRender() {}

  // cleanup() - called when the Scene is no longer needed.
  virtual void cleanup() {}

  // freeze() - when the Manager switches scenes, the previous running scene is
  // calling it's freeze method.
  virtual void freeze() {}

  // invoke() - called when this scene is switching with previous.
  virtual void invoke() {}

  // setExiter(Exiter*) - sets the exiter for scene.
  void setExiter(Exiter* exiter) { this->exiter = exiter; }

  // setSwitcher(Switcher*) - sets the switcher object for this scene.
  void setSwitcher(Switcher* switcher) { this->switcher = switcher; }

  // callExit() - is used to call exit for an application from the scene.
  void callExit() { exiter->exit(); }

  // setNextScene(std::string) - signals to the manager, that after execution
  // this scene's cycle step(update,render,postRender) will be started the
  // process of switching scenes with given scene.
  void setNextScene(std::string scene_name) {
    switcher->switchScene(scene_name);
  }

 private:
  std::string name;

  bool started;

  Switcher* switcher;
  Exiter* exiter;
};

}  // namespace Scenes

#endif  // HPP_CNM_GUI_SCENES_SCENE_HPP
