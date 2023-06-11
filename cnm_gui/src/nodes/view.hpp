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

  ImVec2 getCoords() const noexcept;

 private:
  std::string makeWindowName() const noexcept;

  std::shared_ptr<Cnm::Topology::Common::Node> node_;

  ImVec2 coords_;
};

#endif  // HPP_CNM_GUI_NODES_VIEW_HPP
