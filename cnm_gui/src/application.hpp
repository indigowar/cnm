#ifndef HPP_CNM_GUI_APPLICATION_HPP
#define HPP_CNM_GUI_APPLICATION_HPP

// #include <topology/common/node.hpp>

#include "helpers/menu.hpp"
#include "nodes/view.hpp"

class Application {
 public:
  explicit Application();

  ~Application();

  void render();

 private:
  helpers::Menu makeMenuBar();

  NodeView view_;
  NodeView view1_;

  helpers::Menu menu_;
};

#endif  // HPP_CNM_GUI_APPLICATION_HPP
