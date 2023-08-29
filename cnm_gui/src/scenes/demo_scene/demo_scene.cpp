#include "demo_scene.hpp"

#include <imgui.h>

DemoScene::DemoScene(Scenes::Switcher *switcher, Scenes::Exiter *exiter)
    : Scenes::Scene("demo", switcher, exiter) {}

void DemoScene::start() {}

void DemoScene::update() {}

void DemoScene::render() { ImGui::ShowDemoWindow(); }

void DemoScene::postRender() {}

void DemoScene::cleanup() {}

void DemoScene::freeze() {}

void DemoScene::invoke() {}
