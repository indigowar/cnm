#ifndef HPP_CNM_GUI_APPLICATION_HPP
#define HPP_CNM_GUI_APPLICATION_HPP

// #include <topology/common/node.hpp>

#include <machines/machine.hpp>

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

// class App final {
//  public:
//   ~App();
//
//   void render();
//
//   void add_machine(std::unique_ptr<Cnm::Machines::Machine>);
//
//   App(const App&) = delete;
//   App& operator=(const App&) = delete;
//
//   App(App&&) = delete;
//   App& operator=(App&&) = delete;
//
//  private:
//   const std::unique_ptr<ViewFactory> view_factory_;
//
//   std::unique_ptr<helpers::Menu> menu_;
// };

#endif  // HPP_CNM_GUI_APPLICATION_HPP
