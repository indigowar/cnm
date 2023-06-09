#include <imgui.h>
#include <spdlog/spdlog.h>

#include "application.hpp"

Application::Application() {}

Application::~Application() {}

void Application::makeMenuBar() {
  bool my_tool_active = true;

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("System")) {
      if (ImGui::MenuItem("Exit")) {
        spdlog::info("System->Exit clicked!");
      }
      if (ImGui::MenuItem("Clear")) {
        spdlog::info("System->Clear clicked!");
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Topology")) {
      if (ImGui::MenuItem("Ring")) {
        spdlog::info("Topology->Ring clicked!");
        // TODO: add logic for creating Ring topology
      }

      if (ImGui::MenuItem("Star")) {
        spdlog::info("Topology->Star clicked!");

        // TODO: add logic for creating Star topology
      }

      if (ImGui::MenuItem("Mesh")) {
        spdlog::info("Topology->Mesh clicked!");
        // TODO: add logic for creating Mesh topology
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Machines")) {
      if (ImGui::MenuItem("PC")) {
        spdlog::info("Machines->PC clicked!");
        // TODO: add logic for creating PC Machine
      }

      if (ImGui::MenuItem("Server")) {
        spdlog::info("Machines->Server clicked!");
        // TODO: add logic for creating Server Machine
      }

      if (ImGui::MenuItem("Office Equipment")) {
        spdlog::info("Machines->OfficeEquipment  clicked!");
        // TODO: add logic for creating OfficeEquipment Machine
      }
      ImGui::EndMenu();
    }
  }

  ImGui::EndMainMenuBar();
}
