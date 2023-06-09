#ifndef HPP_CNM_GUI_HELPERS_FPS_WINDOW_HPP
#define HPP_CNM_GUI_HELPERS_FPS_WINDOW_HPP

#include <imgui.h>
#include <spdlog/spdlog.h>

namespace helpers {

void renderFPSWindow() {
  auto is_open{true};
  constexpr ImVec2 size = {100, 10};
  ImGui::Begin("FPS Info", &is_open, ImGuiWindowFlags_NoTitleBar);
  ImGui::Text("FPS: %0.1f", ImGui::GetIO().Framerate);
  ImGui::SetWindowSize(size);
  ImGui::End();
}

}  // namespace helpers

#endif  // HPP_CNM_GUI_HELPERS_FPS_WINDOW_HPP
