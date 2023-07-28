#include "application.hpp"

#include <memory>

#include "scenes/main_scene.hpp"

Application::Application(std::string_view name)
    : scene_manager{},
      exitter(*this),
      should_be_closed{},
      name{std::move(name)} {}

void Application::init() {
  scene_manager = std::make_unique<scene::Manager>(&exitter);

  auto scene = std::make_shared<MainScene>();

  scene_manager->add(scene);
  scene_manager->set_next_scene(scene->get_name());
}

std::string_view Application::getName() const noexcept { return name; }

void Application::update() { scene_manager->update(); }

void Application::render() { scene_manager->render(); }

void Application::post_render() { scene_manager->post_render(); }

void Application::cleanup() { scene_manager->cleanup(); }

bool Application::shouldBeClosed() const noexcept { return should_be_closed; }
