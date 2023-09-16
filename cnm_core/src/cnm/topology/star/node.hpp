#ifndef HPP_CNM_CORE_TOPOLOGY_STAR_NODE_HPP
#define HPP_CNM_CORE_TOPOLOGY_STAR_NODE_HPP

#include <set>

#include "cnm/machine/machine.hpp"
#include "cnm/topology/base/node.hpp"

namespace Cnm::Star {

class Hub;

// Star::Node - is the node in the Star topology.
class Node final : public Cnm::Node {
 public:
  Node(HostInfo, std::unique_ptr<Machine>&&, const std::shared_ptr<Hub>&);

  ~Node() override;

  void start() override;

  void stop() override;

  void invoke() override;

  void freeze() override;

  HostInfo getHostInfo() const noexcept override;

  void setHostInfo(HostInfo) override;

  bool isServing() const noexcept override;

  bool isBusy() const noexcept override;

  std::vector<std::shared_ptr<Cnm::Node>> getConnectedNodes()
      const noexcept override;

  void serve(ServerCtx&&) override;

  std::string_view getType() const noexcept override;

  void attachConnectionNode(Connections::ConnectionNode* ptr) override;

  void detachConnectionNode(Connections::ConnectionNode* ptr) override;

  std::vector<ConnectionInfo> getConnections() const noexcept override;

 private:
  std::unique_ptr<Machine> machine;

  std::shared_ptr<Hub> hub;

  std::set<Cnm::Connections::ConnectionNode*> connection_nodes;

  mutable std::mutex mutex;
};

}  // namespace Cnm::Star

#endif  // HPP_CNM_CORE_TOPOLOGY_STAR_NODE_HPP
