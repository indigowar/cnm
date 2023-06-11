#include "view.hpp"

#include <imgui.h>

#include <sstream>

#include "spdlog/spdlog.h"

using Node = std::shared_ptr<Cnm::Topology::Common::Node>;

void NodeView::render() {
  auto name = makeWindowName();

  if (ImGui::Begin(name.c_str())) {
    if (ImGui::Button("Kill Machine")) {
      spdlog::info("Kill button is called, for ", node_->getAddress());
    }
    if (ImGui::Button("Replace Machine")) {
      spdlog::info("Replace button is called, for ", node_->getAddress());
    }
    ImGui::End();
  }
}

ImVec2 NodeView::getViewCoords() const noexcept {
  // auto name = makeWindowName();
  // ImVec2 coords = ImGui::GetWindowPos(name.data());
  // return coords;
  return {0, 0};
}

std::string NodeView::makeWindowName() const noexcept {
  std::stringstream ss;

  ss << "(" << node_->getAddress() << ") " << node_->getName() << " ("
     << node_->getType() << ")";

  return ss.str();
}
