#include "main_menu.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "lib/render/utils.hpp"

MainMenu::MainMenu() : Scenes::Scene("MainMenu") {}

void MainMenu::render() {
  auto viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowPos(viewport->Pos);

  auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
               ImGuiWindowFlags_NoMove;

  ImGui::Begin("MainMenuWindow", nullptr, flags);

  auto center = calculateWindowCenter(ImGui::GetCurrentWindow());
  auto title_size = ImGui::CalcTextSize("Computer Network Modelling");
  ImGui::SetCursorPos(
      {center.x - title_size.x / 2.0f, center.y - title_size.y / 2.0f});
  ImGui::Text("Computer Network Modelling");

  ImGui::SetCursorPosY(center.y + 25.0f + title_size.y / 2.0f);

  ImGui::SetCursorPosX(
      center.x - ImGui::CalcTextSize("Start with Star Topology").x / 2.0f);

  if (ImGui::Button("Start with Star Topology")) {
    this->setNextScene("StarScene");
  }

  ImGui::SetCursorPosX(
      center.x - ImGui::CalcTextSize("Start with Ring Topology").x / 2.0f);
  if (ImGui::Button("Start with Ring Topology")) {
    this->setNextScene("RingScene");
  }

  ImGui::SetCursorPosX(
      center.x - ImGui::CalcTextSize("Go check out source code").x / 2.0f);
  if (ImGui::Button("Go check out source code")) {
#ifdef _WIN32
    ShellExecute(NULL, "open", "https://github.com/indigowar/cnm/", NULL, NULL,
                 SW_SHOWNORMAL);
#else
    system("open  https://github.com/indigowar/cnm/");
#endif
  }

  ImGui::SetCursorPosX(
      center.x -
      ImGui::CalcTextSize("I would like to leave right now!").x / 2.0f);
  if (ImGui::Button("I would like to leave right now!")) {
    callExit();
  }

  ImGui::End();
}
