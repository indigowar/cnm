#ifndef HPP_CNM_GUI_NODES_PATH_HPP
#define HPP_CNM_GUI_NODES_PATH_HPP

#include <imgui.h>

#include "nodes/view.hpp"

class NodePath {
 public:
  NodePath(NodeView& first, NodeView& second)
      : first_{first}, second_{second} {}

  void render() {
    ImVec2 first_coords = first_.getCoords();
    ImVec2 second_coords = second_.getCoords();

    auto draw_list = ImGui::GetBackgroundDrawList();

    draw_list->AddLine(first_coords, second_coords,
                       ImGui::GetColorU32(ImVec4(160, 160, 160, 1)));
  }

 private:
  NodeView& first_;
  NodeView& second_;
};

#endif  // HPP_CNM_GUI_NODES_PATH_HPP
