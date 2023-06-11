#ifndef HPP_CNM_GUI_NODES_PATH_HPP
#define HPP_CNM_GUI_NODES_PATH_HPP

#include <imgui.h>

#include "nodes/view.hpp"

class NodePath final {
 public:
  NodePath(NodeView& first, NodeView& second)
      : first_{first}, second_{second} {}

  void render() {
    auto first_coords = getCenterOfNodeView(first_);
    auto second_coords = getCenterOfNodeView(second_);

    auto draw_list = ImGui::GetBackgroundDrawList();

    draw_list->AddLine(first_coords, second_coords,
                       ImGui::GetColorU32(ImVec4(160, 160, 160, 1)));
  }

 private:
  ImVec2 getCenterOfNodeView(NodeView& view) {
    auto window_pos = view.getCoords();
    auto window_size = view.getSize();
    return ImVec2(window_pos.x + window_size.x / 2,
                  window_pos.y + window_size.y / 2);
  }

  NodeView& first_;
  NodeView& second_;
};

#endif  // HPP_CNM_GUI_NODES_PATH_HPP
