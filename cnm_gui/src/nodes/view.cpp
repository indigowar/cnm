#include "view.hpp"

#include <imgui.h>

#include <sstream>

#include "spdlog/spdlog.h"

using Node = std::shared_ptr<Cnm::Topology::Common::Node>;

void NodeView::render() {
  auto name = makeWindowName();
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
  if (ImGui::Begin(name.c_str())) {
    retrieveWindowInfo();
    if (ImGui::Button("Kill Machine")) {
      spdlog::info("Kill button is called, for ", node_->getAddress());
    }
    if (ImGui::Button("Replace Machine")) {
      spdlog::info("Replace button is called, for ", node_->getAddress());
    }
    ImGui::End();
  }
  ImGui::PopStyleColor();
}

ImVec2 NodeView::getCoords() const noexcept { return coords_; }
ImVec2 NodeView::getSize() const noexcept { return size_; }

std::string NodeView::makeWindowName() const noexcept {
  std::stringstream ss;

  ss << "(" << node_->getAddress() << ") " << node_->getName() << " ("
     << node_->getType() << ")";

  return ss.str();
}

void NodeView::retrieveWindowInfo() {
  coords_ = ImGui::GetWindowPos();
  size_ = ImGui::GetWindowSize();
}
