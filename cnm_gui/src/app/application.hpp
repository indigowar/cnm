#ifndef HPP_CNM_GUI_APP_APPLICATION_HPP
#define HPP_CNM_GUI_APP_APPLICATION_HPP

#include "lib/scenes/manager.hpp"
#include "lib/scenes/scene.hpp"

class Application final {
  class ApplicationExitter final : public scene::IExitter {
   public:
    ApplicationExitter(Application& app) : app(app) {}

    void exit() override { app.should_be_closed = true; }

   private:
    Application& app;
  };

 public:
  Application();

  void init();
  void update();
  void render();
  void post_render();
  void cleanup();

  bool shouldBeClosed() const noexcept;

 private:
  std::unique_ptr<scene::Manager> scene_manager;

  ApplicationExitter exitter;
  bool should_be_closed;
};

#endif  // HPP_CNM_GUI_APP_APPLICATION_HPP
