#include "node.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <sstream>

#include "cnm/core/object.hpp"

constexpr ImVec4 ACTIVE_NODE_COLOR = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
constexpr ImVec4 INACTIVE_NODE_COLOR = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

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

void renderStopButtonForNode(std::shared_ptr<Cnm::Node>& node) {
  if (ImGui::Button("Stop")) {
    if (node->getStatus() != Cnm::Object::Status::Freezed) {
      node->freeze();
    }
  }
}

void renderKillButtonForNode(std::shared_ptr<Cnm::Node>& node) {
  if (ImGui::Button("Kill")) {
    if (node->getStatus() != Cnm::Object::Status::Dead) {
      node->stop();
    }
  }
}

void renderInvokeButtonForNode(std::shared_ptr<Cnm::Node>& node) {
  if (ImGui::Button("Invoke")) {
    if (node->getStatus() != Cnm::Object::Status::Running) {
      node->invoke();
    }
  }
}

void renderDeleteButtonForNode(std::shared_ptr<Cnm::Node>& node,
                               Cnm::Topology* topology) {
  if (ImGui::Button("Delete")) {
    topology->deleteMachine(node->getHostInfo());
  }
}

void renderMenuForRunningNode(std::shared_ptr<Cnm::Node>& node,
                              Cnm::Topology* topology) {
  IM_ASSERT(node->getStatus() == Cnm::Object::Status::Running);
  renderStopButtonForNode(node);
  ImGui::SameLine();
  renderKillButtonForNode(node);
}

void renderMenuForFrozenNode(std::shared_ptr<Cnm::Node>& node,
                             Cnm::Topology* topology) {
  IM_ASSERT(node->getStatus() == Cnm::Object::Status::Freezed);
  renderInvokeButtonForNode(node);
  ImGui::SameLine();
  renderKillButtonForNode(node);
}

ImGuiWindow* renderNode(std::shared_ptr<Cnm::Node>& node,
                        Cnm::Topology* topology) {
  auto name = makeNodeWindowName(node);

  auto is_active = node->getStatus() == Cnm::Object::Status::Running;

  ImGui::PushStyleColor(ImGuiCol_WindowBg,
                        is_active ? ACTIVE_NODE_COLOR : INACTIVE_NODE_COLOR);

  ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoDocking);

  ImGui::PopStyleColor();

  auto this_window = ImGui::GetCurrentWindow();
  auto parent_window = ImGui::FindWindowByName("Editor");

  IM_ASSERT(parent_window != nullptr);

  auto host_info = node->getHostInfo();

  ImGui::Text("Name: %s", host_info.getName().c_str());
  ImGui::Text("Address: %s", host_info.getAddress().c_str());

  auto status = node->getStatus();

  if (status == Cnm::Object::Status::Running) {
    renderMenuForRunningNode(node, topology);
  } else if (status == Cnm::Object::Status::Freezed) {
    renderMenuForFrozenNode(node, topology);
  } else {
    ImGui::Text("Object is inactive");
    renderDeleteButtonForNode(node, topology);
  }

  // On moving the window, checks if it goes out of Editor window.
  if (ImGui::IsWindowHovered() &&
      ImGui::IsMouseDragging(ImGuiMouseButton_Left, -1.0f)) {
    forceWindowInsideParent(parent_window, this_window);
  }

  ImGui::End();
  return this_window;
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
void renderConnection(ImGuiWindow* first, ImGuiWindow* second, ImGuiCol color) {
  auto first_center = calculateWindowCenter(first);
  auto second_center = calculateWindowCenter(second);

  ImGui::GetWindowDrawList()->AddLine(first_center, second_center, color, 3.0f);
}

void renderNodeConnections(
    const std::vector<std::pair<ImGuiWindow*, ImGuiWindow*>>& cons) {
  for (auto& [f, s] : cons) {
    renderConnection(f, s, IM_COL32(255, 0, 0, 255));
  }
}

std::string makeNodeWindowName(const std::shared_ptr<Cnm::Node>& node) {
  std::stringstream ss;
  ss << node->getHostInfo().getName() << " "
     << node->getHostInfo().getAddress();
  return ss.str();
}
