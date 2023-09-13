#ifndef HPP_CNM_GUI_SCENES_EXITER_HPP
#define HPP_CNM_GUI_SCENES_EXITER_HPP

namespace Scenes {

// Exiter - is an interface for an object that can accept signal for ending the
// program.
class Exiter {
 public:
  // exit() - signals that scene want to end the program.
  virtual void exit() = 0;
};

}  // namespace Scenes

#endif  // HPP_CNM_GUI_SCENES_EXITER_HPP
