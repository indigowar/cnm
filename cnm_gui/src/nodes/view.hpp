#ifndef HPP_CNM_GUI_NODES_VIEW_HPP
#define HPP_CNM_GUI_NODES_VIEW_HPP

#include <imgui.h>

#include <memory>

#include "topology/common/node.hpp"

class NodeView final {
 public:
  explicit NodeView(std::shared_ptr<Cnm::Topology::Common::Node> node)
      : node_{node} {}

  void render();

  ImVec2 getViewCoords() const noexcept;

 private:
  std::string makeWindowName() const noexcept;

  std::shared_ptr<Cnm::Topology::Common::Node> node_;
};

#endif  // HPP_CNM_GUI_NODES_VIEW_HPP
