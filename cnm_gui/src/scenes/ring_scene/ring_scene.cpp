#include "ring_scene.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include <cnm/machine/office_equipment/office_equipment.hpp>
#include <cnm/machine/server/server.hpp>
#include <string_view>

#include "cnm/machine/personal_computer/personal_computer.hpp"
#include "cnm/topology/base/node.hpp"
#include "helpers/fps_window.hpp"
#include "lib/render/node.hpp"

RingScene::RingScene(Scenes::Switcher* switcher, Scenes::Exiter* exiter)
    : Scene("MainScene", switcher, exiter) {
  open_server_creation_window = false;
  open_office_equipment_creation_window = false;
  open_pc_creation_window = false;

  create_server_popup.onSave([this](auto info) {
    auto host_info = Cnm::HostInfo::generate(info.name, info.ip.f, info.ip.s,
                                             info.ip.t, info.ip.fo);

    if (info.type == Popup::CreateServerPopup::ServerType::FileServer &&
        !info.files.has_value()) {
      return;
    }

    std::unique_ptr<Cnm::ServerLogic> logic =
        info.type == Popup::CreateServerPopup::ServerType::FileServer
            ? std::make_unique<Cnm::FileServerLogic>(info.files.value())
            : std::make_unique<Cnm::ServerLogic>();

    auto server = std::make_unique<Cnm::Server>(std::move(logic), info.limit,
                                                host_info, nullptr);

    topology->addMachine(std::move(server), host_info);
  });

  create_office_eq_popup.onSave([this](auto& info) {
    auto host_info = Cnm::HostInfo::generate(info.name, info.ip.f, info.ip.s,
                                             info.ip.t, info.ip.fo);

    std::unique_ptr<Cnm::OfficeEquipmentLogic> logic{};
    if (info.type == Popup::CreateOfficeEquipmentPopup::Type::Scanner) {
      logic = std::make_unique<Cnm::ScannerOfficeEquipmentLogic>(info.base_dir);
    } else {
      logic = std::make_unique<Cnm::PrinterOfficeEquipmentLogic>(info.base_dir);
    }

    auto oe = std::make_unique<Cnm::OfficeEquipment>(std::move(logic),
                                                     host_info, nullptr);
    topology->addMachine(std::move(oe), host_info);
  });

  create_pc_popup.onSave([this](auto& info) {
    auto host_info = Cnm::HostInfo::generate(info.name, info.ip.f, info.ip.s,
                                             info.ip.t, info.ip.fo);
    std::unique_ptr<Cnm::PersonalComputerLogic> logic{};
    if (info.type == Popup::CreatePC::Type::Randomized) {
      logic = std::make_unique<Cnm::RandomPersonalComputerLogic>();
    } else {
      return;
    }
    auto pc = std::make_unique<Cnm::PersonalComputer>(std::move(logic),
                                                      host_info, nullptr);
    topology->addMachine(std::move(pc), host_info);
  });
}

void RingScene::start() {
  spdlog::info("MainScene::start()");

  menu = std::make_unique<Menu::Menu>(makeMenuBar());

  topology = std::make_unique<Cnm::Star::Star>();
  {
    auto server = std::make_unique<Cnm::Server>(
        std::make_unique<Cnm::ServerLogic>(), 10,
        Cnm::HostInfo("Server A", "123.44.50.255"), nullptr);

    topology->addMachine(std::move(server),
                         Cnm::HostInfo("Server A", "123.44.50.255"));

    server = std::make_unique<Cnm::Server>(
        std::make_unique<Cnm::ServerLogic>(), 10,
        Cnm::HostInfo("Server b", "23.122.51.255"), nullptr);

    topology->addMachine(std::move(server),
                         Cnm::HostInfo("Server B", "23.122.51.255"));
  }

  ImGui::StyleColorsDark();
}

void RingScene::update() {}

void RingScene::render() {
  menu->render();
  helpers::renderFPSWindow();

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

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace", nullptr, window_flags);
  ImGui::PopStyleVar(3);

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

  renderEditor();
}

void RingScene::postRender() {}

void RingScene::cleanup() { spdlog::info("TestScene::cleanup()"); }

void RingScene::freeze() { spdlog::info("TestScene::freeze()"); }

void RingScene::invoke() { spdlog::info("TestScene::invoke()"); }

Menu::Menu RingScene::makeMenuBar() {
  const static auto log_msg = [](std::string_view first,
                                 std::string_view second) {
    spdlog::info("{}->{} clicked!", first, second);
  };

  auto app =
      Menu::SubMenu("App", {
                               Menu::Item("Exit",
                                          [this] {
                                            log_msg("App", "Exit");
                                            callExit();
                                          }),
                               Menu::Item("In Main Menu",
                                          [this] {
                                            log_msg("App", "In Main Menu");
                                            setNextScene("MainMenu");
                                          }),
                           });

  auto actions =
      Menu::SubMenu("Actions", {
                                   Menu::Item("Start",
                                              [this] {
                                                log_msg("Actions", "Start");
                                                topology->start();
                                              }),

                                   Menu::Item("Stop",
                                              [this] {
                                                log_msg("Actions", "Stop");
                                                topology->stop();
                                              }),
                                   Menu::Item("Invoke",
                                              [this] {
                                                log_msg("Actions", "Invoke");
                                                topology->invoke();
                                              }),
                                   Menu::Item("Freeze",
                                              [this] {
                                                log_msg("Actions", "Freeze");
                                                topology->freeze();
                                              }),
                               });

  auto add_machine = Menu::SubMenu(
      "Add Machine", {
                         Menu::Item("PC",
                                    [this] {
                                      log_msg("Add Machine", "PC");
                                      open_pc_creation_window = true;
                                    }),
                         Menu::Item("Office Equipment",
                                    [this] {
                                      log_msg("Add Machine", "OfficeEquipment");
                                      open_office_equipment_creation_window =
                                          true;
                                    }),
                         Menu::Item("Server",
                                    [this] {
                                      log_msg("Add Machine", "Server");
                                      open_server_creation_window = true;
                                    }),
                     });

  return Menu::Menu({app, actions, add_machine});
}

void RingScene::renderEditor() {
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

  {
    auto lock = topology->makeLock();
    std::shared_ptr<Cnm::Node> hub = topology->getHub();

    std::vector<std::pair<ImGuiWindow*, ImGuiWindow*>> connections;

    auto hub_window = renderNode(hub, &(*topology));

    std::transform(topology->getHub()->begin(), topology->getHub()->end(),
                   std::back_inserter(connections), [this, hub_window](auto i) {
                     auto win = renderNode(i, &(*topology));
                     return std::pair<ImGuiWindow*, ImGuiWindow*>(hub_window,
                                                                  win);
                   });

    renderNodeConnections(connections);
  }

  create_server_popup.render(&open_server_creation_window);
  create_office_eq_popup.render(&open_office_equipment_creation_window);
  create_pc_popup.render(&open_pc_creation_window);

  ImGui::End();
}