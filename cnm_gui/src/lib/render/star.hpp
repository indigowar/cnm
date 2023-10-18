#ifndef HPP_CNM_GUI_LIB_RENDER_STAR_HPP
#define HPP_CNM_GUI_LIB_RENDER_STAR_HPP

#include <imgui.h>
#include <imgui_internal.h>

#include <memory>

#include "cnm/topology/star/node.hpp"
#include "cnm/topology/star/star.hpp"

ImGuiWindow* renderStarNode(std::shared_ptr<Cnm::Star::Node>& node,
                            Cnm::Star::Star* topology);

ImGuiWindow* renderStarHub(std::shared_ptr<Cnm::Star::Hub>& hub,
                           Cnm::Star::Star* topology);

#endif  // HPP_CNM_GUI_LIB_RENDER_STAR_HPP
