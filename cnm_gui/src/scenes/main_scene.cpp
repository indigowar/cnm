#include "main_scene.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include <string_view>

#include "helpers/fps_window.hpp"

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

  static auto dock_space_flags =
      ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoTabBar;

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
      ImGui::DockBuilderRemoveNode(dock_space_id);  // clear any previous
      ImGui::DockBuilderAddNode(
          dock_space_id, dock_space_flags | ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dock_space_id, viewport->Size);

      ImGuiID left{}, right{};
      right = ImGui::DockBuilderSplitNode(dock_space_id, ImGuiDir_Right, 0.2f,
                                          &right, &left);

      ImGui::DockBuilderDockWindow("Editor", left);
      ImGui::DockBuilderDockWindow("Properties", right);
      ImGui::DockBuilderFinish(dock_space_id);
    }
  }
  ImGui::End();

  render_props();
  render_editor();
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

void render_node(const std::string& name) {
  ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoDocking);

  auto this_window = ImGui::GetCurrentWindow();
  auto parent_window = ImGui::FindWindowByName("Editor");

  IM_ASSERT(parent_window != nullptr);

  // On moving the window, checks if it goes out of Editor window.
  if (ImGui::IsWindowHovered() &&
      ImGui::IsMouseDragging(ImGuiMouseButton_Left, -1.0f)) {
    auto this_pos = ImGui::GetWindowPos();
    auto this_size = ImGui::GetWindowSize();

    auto parent_pos = parent_window->Pos;
    auto parent_size = parent_window->Size;

    if (parent_pos.x > this_pos.x) {
      this_pos.x = parent_pos.x;
    }

    if (parent_pos.y > this_pos.y) {
      this_pos.y = parent_pos.y;
    }

    if (parent_pos.x + parent_size.x < this_pos.x + this_size.x) {
      this_pos.x = parent_pos.x + parent_size.x - this_size.x;
    }

    if (parent_pos.y + parent_size.y < this_pos.y + this_size.y) {
      this_pos.y = parent_pos.y + parent_size.y - this_size.y;
    }

    ImGui::SetWindowPos(this_pos);
  }

  ImGui::End();
}

void MainScene::render_editor() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

  ImGui::Begin("Editor", nullptr,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

  ImGui::PopStyleVar();

  static ImVec2 scrolling(0.0f, 0.0f);

  auto canvas_begin = ImGui::GetCursorScreenPos();
  auto canvas_size = ImGui::GetContentRegionAvail();

  if (canvas_size.x < 100.0f) canvas_size.x = 100.0f;
  if (canvas_size.y < 100.0f) canvas_size.y = 100.0f;

  ImVec2 canvas_end(canvas_begin.x + canvas_size.x,
                    canvas_begin.y + canvas_size.y);

  auto& io = ImGui::GetIO();
  auto draw_list = ImGui::GetWindowDrawList();
  draw_list->AddRectFilled(canvas_begin, canvas_end, IM_COL32(50, 50, 50, 255));
  draw_list->AddRect(canvas_begin, canvas_end, IM_COL32_BLACK);

  ImGui::InvisibleButton(
      "canvas", canvas_size,
      ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

  const bool is_hovered = ImGui::IsItemHovered();
  const bool is_active = ImGui::IsItemActive();

  // draw grid
  draw_list->PushClipRect(canvas_begin, canvas_end, true);
  const auto GRID_STEP = 64.0f;
  for (auto x = fmodf(scrolling.x, GRID_STEP); x < canvas_size.x;
       x += GRID_STEP) {
    draw_list->AddLine(ImVec2(canvas_begin.x + x, canvas_begin.y),
                       ImVec2(canvas_begin.x + x, canvas_end.y),
                       IM_COL32(200, 200, 200, 40));
  }

  for (auto y = fmodf(scrolling.y, GRID_STEP); y < canvas_size.y;
       y += GRID_STEP) {
    draw_list->AddLine(ImVec2(canvas_begin.x, canvas_begin.y + y),
                       ImVec2(canvas_end.x, canvas_begin.y + y),
                       IM_COL32(200, 200, 200, 40));
  }

  render_node("Node A");

  render_node("Node B");

  render_node("Node C");

  ImGui::End();
}

void MainScene::render_props() {
  ImGui::Begin("Properties", nullptr,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoResize);

  ImGui::End();
}
