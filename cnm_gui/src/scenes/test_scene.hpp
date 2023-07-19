#ifndef HPP_TEST_SCENE_HPP
#define HPP_TEST_SCENE_HPP

#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include <machines/server.hpp>
#include <memory>
#include <topology/ring/ring.hpp>

#include "lib/scenes/scene.hpp"

class TestScene final : public scene::Scene {
 public:
  TestScene(scene::ISceneSwitcher* switcher = nullptr,
            scene::IExitter* exitter = nullptr)
      : Scene("test_scene", switcher, exitter) {}

  void start() override {
    spdlog::info("TestScene::start()");

    topology_ = std::make_unique<Cnm::Topology::Ring::Topology>();
    {
      auto logic = [](const Cnm::Machines::HostInfo& info,
                      std::unique_ptr<Cnm::Connection::ServerCtx>&& ctx) {};
    }
  }

  void update() override {}

  void render() override {}

  void post_render() override {}

  void cleanup() override { spdlog::info("TestScene::cleanup()"); }

  void froze() override { spdlog::info("TestScene::froze()"); }

  void invoke() override { spdlog::info("TestScene::invoke()"); }

 private:
  std::unique_ptr<Cnm::Topology::Ring::Topology> topology_;
};

#endif  // HPP_TEST_SCENE_HPP;
