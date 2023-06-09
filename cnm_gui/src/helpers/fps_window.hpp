#ifndef HPP_CNM_GUI_HELPERS_FPS_WINDOW_HPP
#define HPP_CNM_GUI_HELPERS_FPS_WINDOW_HPP

#include <imgui.h>

namespace helpers {

void renderFPSWindow() {
  ImGui::Begin("FPS Info");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();
}

}  // namespace helpers

#endif  // HPP_CNM_GUI_HELPERS_FPS_WINDOW_HPP
