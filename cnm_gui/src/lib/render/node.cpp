#include "node.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <sstream>

#include "lib/render/ring.hpp"
#include "lib/render/star.hpp"
#include "lib/render/utils.hpp"

ImGuiWindow* renderNode(std::shared_ptr<Cnm::Node>& node,
                        Cnm::Topology* topology) {
  if (topology->getType() == Cnm::Star::Star::TYPE) {
    auto star_topology = dynamic_cast<Cnm::Star::Star*>(topology);
    if (node->getType() == Cnm::Star::Hub::TYPE) {
      std::shared_ptr<Cnm::Star::Hub> hub =
          std::dynamic_pointer_cast<Cnm::Star::Hub>(node);
      return renderStarHub(hub, star_topology);
    }
    std::shared_ptr<Cnm::Star::Node> star_node =
        std::dynamic_pointer_cast<Cnm::Star::Node>(node);
    return renderStarNode(star_node, star_topology);
  }

  if (topology->getType() == Cnm::Ring::Ring::TYPE) {
    auto ring_topology = dynamic_cast<Cnm::Ring::Ring*>(topology);
    auto ring_node = std::dynamic_pointer_cast<Cnm::Ring::Node>(node);
    return renderRingNode(ring_node, ring_topology);
  }
  return nullptr;
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
