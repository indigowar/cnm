#ifndef HPP_CNM_GUI_RENDER_NODE_HPP
#define HPP_CNM_GUI_RENDER_NODE_HPP

#include <cnm/topology/base/node.hpp>

void renderNode(std::shared_ptr<Cnm::Node>&);

void renderNodeConnections(const std::shared_ptr<Cnm::Node>&);

std::string makeNodeWindowName(const std::shared_ptr<Cnm::Node>&);

#endif  // HPP_CNM_GUI_RENDER_NODE_HPP
