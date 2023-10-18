#include "fps_window.hpp"

namespace helpers {

void renderFPSWindow() {
  auto flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
      ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

  const float PAD = 100.0f;
  auto viewport = ImGui::GetMainViewport();

  auto work_size = viewport->WorkSize;
  auto work_pos = viewport->WorkPos;

  auto pos =
      ImVec2(work_pos.x + work_size.x - PAD, work_pos.y + work_size.y - PAD);
  auto pivot = ImVec2(0.0f, 0.0f);

  ImGui::SetNextWindowPos(pos, ImGuiCond_Always, pivot);
  ImGui::SetNextWindowBgAlpha(0.35f);

  if (ImGui::Begin("FPS Overlay", nullptr, flags)) {
    ImGui::Text("FPS: %0.1f", ImGui::GetIO().Framerate);
  }
  ImGui::End();
}

}  // namespace helpers
