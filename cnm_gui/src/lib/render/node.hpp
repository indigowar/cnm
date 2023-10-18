#ifndef HPP_CNM_GUI_RENDER_NODE_HPP
#define HPP_CNM_GUI_RENDER_NODE_HPP

#include <cnm/topology/base/node.hpp>
#include <cnm/topology/base/topology.hpp>

class ImGuiWindow;

ImGuiWindow* renderNode(std::shared_ptr<Cnm::Node>&, Cnm::Topology*);

void renderNodeConnections(
    const std::vector<std::pair<ImGuiWindow*, ImGuiWindow*>>& values);

#endif  // HPP_CNM_GUI_RENDER_NODE_HPP
