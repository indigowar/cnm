#include "application.hpp"

#include "scenes/main_menu/main_menu.hpp"
#include "scenes/ring_scene/ring_scene.hpp"
#include "scenes/star_scene/star_scene.hpp"

Application::Application(std::string name)
    : scene_manager(),
      exiter(*this),
      should_be_closed{},
      name(std::move(name)) {}

void Application::init() {
  scene_manager = std::make_unique<Scenes::Manager>(&exiter);

  auto star = std::make_shared<StarScene>();
  auto ring = std::make_shared<RingScene>();
  auto menu = std::make_shared<MainMenu>();

  scene_manager->add(star);
  scene_manager->add(ring);
  scene_manager->add(menu);

  scene_manager->setNextScene(menu->getName());
}

const std::string& Application::getName() const noexcept { return name; }

void Application::update() { scene_manager->update(); }

void Application::render() { scene_manager->render(); }

void Application::postRender() { scene_manager->postRender(); }

void Application::cleanup() { scene_manager->cleanup(); }

bool Application::shouldBeClosed() const noexcept { return should_be_closed; }
