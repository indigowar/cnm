#include "main_scene.hpp"

#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include <utility>

#include "helpers/fps_window.hpp"

void test() {}

MainScene::MainScene(scene::ISceneSwitcher* switcher, scene::IExitter* exitter)
    : Scene("test_scene", switcher, exitter) {}

void MainScene::start() {
  spdlog::info("TestScene::start()");

  menu = std::make_unique<helpers::Menu>(makeMenuBar());

  ImGui::StyleColorsDark();
}

void MainScene::update() {}

void MainScene::render() {
  menu->render();
  helpers::renderFPSWindow();
  // docking over all window

  static auto dock_space_flags = ImGuiDockNodeFlags_PassthruCentralNode;

  static auto window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

  auto viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  if (dock_space_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
    window_flags |= ImGuiWindowFlags_NoBackground;
  }

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace", nullptr, window_flags);
  ImGui::PopStyleVar();
  ImGui::PopStyleVar(2);

  auto& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    auto dock_space_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dock_space_id, ImVec2(0.0f, 0.0f), dock_space_flags);

    static auto first_time = true;
    if (first_time) {
      first_time = false;
      ImGui::DockBuilderRemoveNode(dock_space_id);  // clear any previous layout
      ImGui::DockBuilderAddNode(
          dock_space_id, dock_space_flags | ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dock_space_id, viewport->Size);

      ImGuiID left{}, right{};
      right = ImGui::DockBuilderSplitNode(dock_space_id, ImGuiDir_Right, 0.3f,
                                          &right, &left);

      ImGui::DockBuilderDockWindow("Editor", left);
      ImGui::DockBuilderDockWindow("Properties", right);
      ImGui::DockBuilderFinish(dock_space_id);
    }
  }
  ImGui::End();

  ImGui::Begin("Editor");
  ImGui::End();

  ImGui::Begin("Properties");
  ImGui::End();

  //  ImGui::ShowDemoWindow();
}

void MainScene::post_render() {}

void MainScene::cleanup() { spdlog::info("TestScene::cleanup()"); }

void MainScene::froze() { spdlog::info("TestScene::freeze()"); }

void MainScene::invoke() { spdlog::info("TestScene::invoke()"); }

helpers::Menu MainScene::makeMenuBar() {
  /**
   * TODO: Add the actual logic for every MenuField
   */

  const static auto test = [](std::string_view first, std::string_view second) {
    spdlog::info("{}->{} clicked!", first, second);
  };

  auto app = helpers::MenuItem(
      "Applicaition",
      {helpers::MenuField("Exit", std::bind(test, "Application", "Close")),
       helpers::MenuField("Clear", std::bind(test, "Application", "Clear"))});

  auto view = helpers::MenuItem(
      "View", {helpers::MenuField("Test", std::bind(test, "View", "Test"))});

  auto topology = helpers::MenuItem(
      "Topology",
      {helpers::MenuField("Ring", std::bind(test, "Topology", "Ring")),
       helpers::MenuField("Star", std::bind(test, "Topology", "Star")),
       helpers::MenuField("Mesh", std::bind(test, "Topology", "Mesh"))});

  auto machine = helpers::MenuItem(
      "Machine",
      {helpers::MenuField("PC", std::bind(test, "Machine", "PC")),
       helpers::MenuField("Server", std::bind(test, "Machine", "Server")),
       helpers::MenuField("Office Equipment",
                          std::bind(test, "Machine", "Office Equipment"))});

  return helpers::Menu({app, view, topology, machine});
}
