#ifndef HPP_CNM_GUI_LIB_RENDER_RING_HPP
#define HPP_CNM_GUI_LIB_RENDER_RING_HPP

#include <imgui.h>
#include <imgui_internal.h>

#include "cnm/topology/ring/node.hpp"
#include "cnm/topology/ring/ring.hpp"

ImGuiWindow* renderRingNode(std::shared_ptr<Cnm::Ring::Node>& node,
                            Cnm::Ring::Ring* ring);

#endif  // HPP_CNM_GUI_LIB_RENDER_RING_HPP
