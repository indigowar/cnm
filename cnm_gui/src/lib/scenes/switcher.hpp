#ifndef HPP_CNM_GUI_SCENES_SWITCHER_HPP
#define HPP_CNM_GUI_SCENES_SWITCHER_HPP

#include <string>

namespace Scenes {

// Switcher - is an interface to an object that can switch scenes.
class Switcher {
 public:
  // switchScene(std::string) - specifies the next scene by it's name.
  virtual void switchScene(const std::string& scene_name) = 0;
};

}  // namespace Scenes

#endif  // HPP_CNM_GUI_SCENES_SWITCHER_HPP
