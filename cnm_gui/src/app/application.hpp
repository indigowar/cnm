#ifndef HPP_CNM_GUI_APP_APPLICATION_HPP
#define HPP_CNM_GUI_APP_APPLICATION_HPP

#include <memory>

#include "lib/scenes/exiter.hpp"
#include "lib/scenes/manager.hpp"

class Application final {
 public:
  Application(std::string name);

  [[nodiscard]] const std::string& getName() const noexcept;

  void init();

  void update();

  void render();

  void postRender();

  void cleanup();

  [[nodiscard]] bool shouldBeClosed() const noexcept;

 private:
  class AppExiter final : public Scenes::Exiter {
    Application& app;

   public:
    AppExiter(Application& a) : app{a} {}

    void exit() override { app.should_be_closed = true; }
  };

  std::unique_ptr<Scenes::Manager> scene_manager;
  bool should_be_closed;
  AppExiter exiter;
  std::string name;
};

#endif  // HPP_CNM_GUI_APP_APPLICATION_HPP
