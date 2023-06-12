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
  ImVec2 getSize() const noexcept;

 private:
  std::string makeWindowName() const noexcept;

  void retrieveWindowInfo();

  std::shared_ptr<Cnm::Topology::Common::Node> node_;

  ImVec2 coords_;
  ImVec2 size_;
};

class View {
 public:
  virtual void render() = 0;
  virtual ImVec2 getCoords() const noexcept = 0;
  virtual ImVec2 getSize() const noexcept = 0;

  virtual void setCoords(ImVec2) const noexcept = 0;
  virtual void setSize(ImVec2) const noexcept = 0;

  std::vector<std::shared_ptr<Cnm::Topology::Common::Node>> getConnectedNodes()
      const noexcept;
};

class ViewFactory {
 public:
  virtual View make(
      std::shared_ptr<Cnm::Topology::Common::Node>) const noexcept = 0;
};

#endif  // HPP_CNM_GUI_NODES_VIEW_HPP
