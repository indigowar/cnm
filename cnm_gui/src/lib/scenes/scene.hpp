#ifndef HPP_CNM_GUI_SCENES_SCENE_HPP
#define HPP_CNM_GUI_SCENES_SCENE_HPP

#include <string_view>

namespace scene {

// ISceneSwitcher - is interface to an object that can switch the scene.
// It is in-use inside Scene class for specifying the next scene.
class ISceneSwitcher {
 public:
  // switch_scene - specifies the next scene that will be active by it's name
  virtual void switch_scene(std::string_view scene_name) = 0;
};

// IExitter - is an object that can handle the exit initiated inside the Scene
class IExitter {
 public:
  virtual void exit() = 0;
};

// Scene is an object that contains state and logic for specific "sub-program",
// that can be loaded into main application.
class Scene {
 public:
  Scene(std::string_view name, ISceneSwitcher* switcher = nullptr,
        IExitter* exitter = nullptr)
      : name{name}, scene_switcher{switcher}, exitter{exitter}, started{} {}

  virtual ~Scene() = default;

  // get_name() - returns the name of this scene
  std::string_view get_name() const noexcept { return name; }

  // has_started() - returns true, if the start() method has been called
  bool has_started() const noexcept { return started; }

  virtual void call_start() final {
    start();
    started = true;
  }

  virtual void start() {}

  virtual void update() {}

  virtual void render() {}

  virtual void post_render() {}

  virtual void cleanup() {}

  virtual void froze() {}

  virtual void invoke() {}

  void set_exitter(IExitter* exitter) { this->exitter = exitter; }
  void set_switcher(ISceneSwitcher* switcher) { scene_switcher = switcher; }

  void call_exit_program() { exitter->exit(); }

  void specify_next_scene(std::string_view scene_name) {
    scene_switcher->switch_scene(scene_name);
  }

 private:
  std::string_view name;

  bool started;

  ISceneSwitcher* scene_switcher;
  IExitter* exitter;
};

}  // namespace scene

#endif  // HPP_CNM_GUI_SCENES_SCENE_HPP
