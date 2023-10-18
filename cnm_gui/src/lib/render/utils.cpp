#include "utils.hpp"

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

std::string makeNodeWindowName(const std::shared_ptr<Cnm::Node>& node) {
  std::stringstream ss;
  ss << node->getHostInfo().getName() << " "
     << node->getHostInfo().getAddress();
  return ss.str();
}

ImVec2 calculateWindowCenter(const ImGuiWindow* window) {
  auto pos = window->Pos;
  auto size = window->Size;
  return {pos.x + (size.x / 2), pos.y + (size.y / 2)};
}
