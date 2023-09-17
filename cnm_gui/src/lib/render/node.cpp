#include "node.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <sstream>

// forceWindowInsideParent - force the child to be inside the parent window.
void forceWindowInsideParent(const ImGuiWindow* parent, ImGuiWindow* child) {
  auto child_pos = child->Pos;
  auto child_size = child->Size;

  auto parent_pos = parent->Pos;
  auto parent_size = parent->Size;

  if (parent_pos.x > child_pos.x) {
    child_pos.x = parent_pos.x;
  }

  if (parent_pos.y > child_pos.y) {
    child_pos.y = parent_pos.y;
  }

  if (parent_pos.x + parent_size.x < child_pos.x + child_size.x) {
    child_pos.x = parent_pos.x + parent_size.x - child_size.x;
  }

  if (parent_pos.y + parent_size.y < child_pos.y + child_size.y) {
    child_pos.y = parent_pos.y + parent_size.y - child_size.y;
  }

  child->Pos = child_pos;
}

void renderNode(std::shared_ptr<Cnm::Node>& node) {
  auto name = makeNodeWindowName(node);

  ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoDocking);

  auto this_window = ImGui::GetCurrentWindow();
  auto parent_window = ImGui::FindWindowByName("Editor");

  IM_ASSERT(parent_window != nullptr);

  // On moving the window, checks if it goes out of Editor window.
  if (ImGui::IsWindowHovered() &&
      ImGui::IsMouseDragging(ImGuiMouseButton_Left, -1.0f)) {
    forceWindowInsideParent(parent_window, this_window);
  }

  ImGui::End();
}

// calculateWindowCenter - calculates the center of the window based of it's
// size and position.
ImVec2 calculateWindowCenter(const ImGuiWindow* window) {
  auto pos = window->Pos;
  auto size = window->Size;
  return {pos.x + (size.x / 2), pos.y + (size.y / 2)};
}

// renderConnection - renders the connection between two windows with given
// color.
void renderConnection(const char* first_name, const char* second_name,
                      ImGuiCol color) {
  auto first = ImGui::FindWindowByName(first_name);
  auto second = ImGui::FindWindowByName(second_name);

  auto first_center = calculateWindowCenter(first);
  auto second_center = calculateWindowCenter(second);

  ImGui::GetWindowDrawList()->AddLine(first_center, second_center, color, 3.0f);
}

void renderNodeConnections(const std::shared_ptr<Cnm::Node>& node) {
  auto connected_nodes = node->getConnectedNodes();

  for (const auto& i : connected_nodes) {
    renderConnection(makeNodeWindowName(node).c_str(),
                     makeNodeWindowName(i).c_str(), IM_COL32(255, 0, 0, 255));
  }
}

std::string makeNodeWindowName(const std::shared_ptr<Cnm::Node>& node) {
  std::stringstream ss;
  ss << node->getHostInfo().getName() << " "
     << node->getHostInfo().getAddress();
  return ss.str();
}