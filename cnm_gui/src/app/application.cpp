#include "application.hpp"

#include "scenes/demo_scene/demo_scene.hpp"
#include "scenes/main_scene/main_scene.hpp"

Application::Application(std::string name)
    : scene_manager(),
      exiter(*this),
      should_be_closed{},
      name(std::move(name)) {}

void Application::init() {
  scene_manager = std::make_unique<Scenes::Manager>(&exiter);

  auto main_scene = std::make_shared<MainScene>();
  scene_manager->add(main_scene);

  auto demo_scene = std::make_shared<DemoScene>();
  scene_manager->add(demo_scene);

  scene_manager->setNextScene(demo_scene->getName());
}

const std::string& Application::getName() const noexcept { return name; }

void Application::update() { scene_manager->update(); }

void Application::render() { scene_manager->render(); }

void Application::postRender() { scene_manager->postRender(); }

void Application::cleanup() { scene_manager->cleanup(); }

bool Application::shouldBeClosed() const noexcept { return should_be_closed; }
