#ifndef HPP_CNM_CORE_TOPOLOGY_RING_RING_NODE_HPP
#define HPP_CNM_CORE_TOPOLOGY_RING_RING_NODE_HPP

#include <memory>
#include <mutex>
#include <set>

#include "cnm/machine/communicator.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/topology/base/node.hpp"

namespace Cnm {

class RingNode final : public Node {
 public:
  RingNode(HostInfo, std::unique_ptr<Machine>&&,
           const std::shared_ptr<Communicator>&);

  ~RingNode() override;

  [[nodiscard]] HostInfo getHostInfo() const noexcept override;

  void setHostInfo(HostInfo) override;

  std::vector<std::shared_ptr<Node>> getConnectedNodes()
      const noexcept override;

  bool isServing() const noexcept override;

  bool isBusy() const noexcept;

  [[nodiscard]] std::shared_ptr<RingNode> getPreviousNode() const noexcept;

  void setPreviousNode(std::shared_ptr<RingNode>);

  [[nodiscard]] std::shared_ptr<RingNode> getNextNode() const noexcept;

  void setNextNode(std::shared_ptr<RingNode>);

  void start() override;
  void stop() override;
  void invoke() override;
  void freeze() override;

  void serve(std::unique_ptr<ServerContext>&&) override;

  std::string_view getType() const noexcept override;

  void attachConnectionNode(Connections::ConnectionNode* ptr) override {
    if (!connection_nodes.contains(ptr)) {
      connection_nodes.emplace(ptr);
    }
  }
  void detachConnectionNode(Connections::ConnectionNode* ptr) override {
    if (connection_nodes.contains(ptr)) {
      connection_nodes.erase(ptr);
    }
  }

  std::vector<ConnectionInfo> getConnections() const noexcept override {
    std::vector<ConnectionInfo> result{};
    std::transform(connection_nodes.begin(), connection_nodes.end(),
                   std::back_inserter(result),
                   [](Connections::ConnectionNode* ptr) -> ConnectionInfo {
                     return {ptr->getOwner().getClientHostInfo(),
                             ptr->getOwner().getServerHostInfo()};
                   });
    return result;
  }

 private:
  std::shared_ptr<RingNode> previous_node;
  std::shared_ptr<RingNode> next_node;

  std::unique_ptr<Machine> machine;

  mutable std::mutex mutex;

  std::set<Cnm::Connections::ConnectionNode*> connection_nodes;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_RING_RING_NODE_HPP
