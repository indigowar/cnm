#include "star.hpp"

#include "cnm/topology/star/node.hpp"
#include "lib/render/utils.hpp"

ImGuiWindow* renderStarNode(std::shared_ptr<Cnm::Star::Node>& node,
                            Cnm::Star::Star* topology) {
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

  std::shared_ptr<Cnm::Node> base_ptr =
      std::static_pointer_cast<Cnm::Node>(node);

  if (status == Cnm::Object::Status::Running) {
    renderMenuForRunningNode(base_ptr, topology);
  } else if (status == Cnm::Object::Status::Freezed) {
    renderMenuForFrozenNode(base_ptr, topology);
  } else {
    ImGui::Text("Object is inactive");
    setCursorAtTheBottom(35);
    renderDeleteButtonForNode(base_ptr, topology);
  }

  // On moving the window, checks if it goes out of Editor window.
  if (ImGui::IsWindowHovered() &&
      ImGui::IsMouseDragging(ImGuiMouseButton_Left, -1.0f)) {
    forceWindowInsideParent(parent_window, this_window);
  }

  ImGui::End();
  return this_window;
}

ImGuiWindow* renderStarHub(std::shared_ptr<Cnm::Star::Hub>& hub,
                           Cnm::Star::Star* topology) {
  std::string name = "HUB";

  auto is_active = hub->getStatus() == Cnm::Object::Status::Running;

  ImGui::PushStyleColor(ImGuiCol_WindowBg,
                        is_active ? ACTIVE_NODE_COLOR : INACTIVE_NODE_COLOR);

  ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoDocking);

  ImGui::PopStyleColor();

  auto this_window = ImGui::GetCurrentWindow();
  auto parent_window = ImGui::FindWindowByName("Editor");

  IM_ASSERT(parent_window != nullptr);

  auto host_info = hub->getHostInfo();

  ImGui::Text("Name: %s", host_info.getName().c_str());
  ImGui::Text("Address: %s", host_info.getAddress().c_str());

  ImGui::Text("Count of nodes: %zu", hub->getConnectedNodes().size());

  auto status = hub->getStatus();

  std::shared_ptr<Cnm::Node> base_ptr =
      std::static_pointer_cast<Cnm::Node>(hub);

  if (status == Cnm::Object::Status::Running) {
    renderMenuForRunningNode(base_ptr, topology);
  } else if (status == Cnm::Object::Status::Freezed) {
    renderMenuForFrozenNode(base_ptr, topology);
  } else {
    ImGui::Text("Object is inactive");
    // The Hub should not be ever deleted.
    //    renderDeleteButtonForNode(base_ptr, topology);
  }

  // On moving the window, checks if it goes out of Editor window.
  if (ImGui::IsWindowHovered() &&
      ImGui::IsMouseDragging(ImGuiMouseButton_Left, -1.0f)) {
    forceWindowInsideParent(parent_window, this_window);
  }

  ImGui::End();
  return this_window;
}