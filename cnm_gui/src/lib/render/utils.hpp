#ifndef HPP_CNM_GUI_LIB_RENDER_UTILS_HPP
#define HPP_CNM_GUI_LIB_RENDER_UTILS_HPP

#include <imgui.h>
#include <imgui_internal.h>

#include <memory>

#include "cnm/topology/base/node.hpp"
#include "cnm/topology/base/topology.hpp"

// forceWindowInsideParent - force the child to be inside the parent window.
void forceWindowInsideParent(const ImGuiWindow* parent, ImGuiWindow* child);

void renderStopButtonForNode(std::shared_ptr<Cnm::Node>& node);

void renderKillButtonForNode(std::shared_ptr<Cnm::Node>& node);

void renderInvokeButtonForNode(std::shared_ptr<Cnm::Node>& node);

void renderDeleteButtonForNode(std::shared_ptr<Cnm::Node>& node,
                               Cnm::Topology* topology);

void renderMenuForRunningNode(std::shared_ptr<Cnm::Node>& node,
                              Cnm::Topology* topology);

void renderMenuForFrozenNode(std::shared_ptr<Cnm::Node>& node,
                             Cnm::Topology* topology);

constexpr ImVec4 ACTIVE_NODE_COLOR = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
constexpr ImVec4 INACTIVE_NODE_COLOR = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

std::string makeNodeWindowName(const std::shared_ptr<Cnm::Node>& node);

// calculateWindowCenter - calculates the center of the window based of it's
// size and position.
ImVec2 calculateWindowCenter(const ImGuiWindow* window);

void setCursorAtTheBottom(float offset);

#endif  // HPP_CNM_GUI_LIB_RENDER_UTILS_HPP
