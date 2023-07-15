#ifndef HPP_TEST_SCENE_HPP
#define HPP_TEST_SCENE_HPP

#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

#include "lib/scenes/scene.hpp"

class TestScene final : public scene::Scene {
 public:
  TestScene(scene::ISceneSwitcher* switcher = nullptr,
            scene::IExitter* exitter = nullptr)
      : Scene("test_scene", switcher, exitter) {}

  void start() override { spdlog::info("TestScene::start()"); }

  void update() override {}

  void render() override {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(),
                                 ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::Begin("lfw");
    ImGui::End();

    ImGui::DockBuilderDockWindow("lfw", ImGuiDir_Left);
    ImGui::Begin("bpw");
    ImGui::End();
  }

  void post_render() override {}

  void cleanup() override { spdlog::info("TestScene::cleanup()"); }

  void froze() override { spdlog::info("TestScene::froze()"); }

  void invoke() override { spdlog::info("TestScene::invoke()"); }
};

#endif  // HPP_TEST_SCENE_HPP;
