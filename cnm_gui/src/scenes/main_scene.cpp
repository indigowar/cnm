#include "main_scene.hpp"

#include <spdlog/spdlog.h>

#include "helpers/fps_window.hpp"
#include "lib/nodes/imnodes.hpp"

MainScene::MainScene(scene::ISceneSwitcher* switcher, scene::IExitter* exitter)
    : Scene("test_scene", switcher, exitter) {}

void MainScene::start() {
  spdlog::info("TestScene::start()");

  menu = std::make_unique<helpers::Menu>(makeMenuBar());

  ImNodes::CreateContext();

  ImNodes::SetNodeGridSpacePos(1, ImVec2(200.0f, 200.0f));

  ImGui::StyleColorsDark();
  ImNodes::StyleColorsDark();
}

void MainScene::update() {}

void MainScene::render() {
  menu->render();

  // docking
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

  ImGui::Begin("Node Editor");

  ImGui::SetNextWindowDockID(ImGuiID(0), ImGuiCond_Always);

  ImNodes::BeginNodeEditor();
  ImNodes::BeginNode(1);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("test:)");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(2);
  ImGui::Text("left");
  ImNodes::EndInputAttribute();

  ImNodes::BeginOutputAttribute(3);
  ImGui::Indent(40);
  ImGui::Text("right");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();

  ImNodes::BeginNode(2);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("hello");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(5);
  ImGui::Text("left");
  ImNodes::EndInputAttribute();

  ImNodes::BeginOutputAttribute(6);
  ImGui::Indent(40);
  ImGui::Text("right");
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();

  ImNodes::EndNodeEditor();

  ImGui::End();

  ImGui::Begin("hello, world");
  ImGui::Text("Hey");
  ImGui::Button("SHAAAA!");
  ImGui::End();

  helpers::renderFPSWindow();
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
