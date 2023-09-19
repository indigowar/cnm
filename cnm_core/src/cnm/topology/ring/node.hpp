#ifndef HPP_CNM_CORE_TOPOLOGY_RING_RING_NODE_HPP
#define HPP_CNM_CORE_TOPOLOGY_RING_RING_NODE_HPP

#include <memory>
#include <mutex>
#include <set>

#include "cnm/machine/communicator.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/topology/base/node.hpp"

namespace Cnm::Ring {

class Node final : public Cnm::Node {
 public:
  Node(HostInfo, std::unique_ptr<Machine>&&, std::unique_ptr<Communicator>&&);

  ~Node() override;

  [[nodiscard]] HostInfo getHostInfo() const noexcept override;

  void setHostInfo(HostInfo) override;

  std::vector<std::shared_ptr<Cnm::Node>> getConnectedNodes()
      const noexcept override;

  bool isServing() const noexcept override;

  bool isBusy() const noexcept override;

  [[nodiscard]] std::shared_ptr<Node> getPreviousNode() const noexcept;

  void setPreviousNode(std::shared_ptr<Node>);

  [[nodiscard]] std::shared_ptr<Node> getNextNode() const noexcept;

  void setNextNode(std::shared_ptr<Node>);

  void start() override;

  void stop() override;

  void invoke() override;

  void freeze() override;

  Object::Status getStatus() const noexcept override {
    return machine->getStatus();
  }

  void serve(std::unique_ptr<ServerContext>&&) override;

  std::string_view getType() const noexcept override;

  void attachConnectionNode(Connections::ConnectionNode* ptr) override;

  void detachConnectionNode(Connections::ConnectionNode* ptr) override;

  std::vector<ConnectionInfo> getConnections() const noexcept override;

 private:
  std::shared_ptr<Node> previous_node;
  std::shared_ptr<Node> next_node;

  std::unique_ptr<Machine> machine;

  mutable std::mutex mutex;

  std::set<Cnm::Connections::ConnectionNode*> connection_nodes;
};

}  // namespace Cnm::Ring

#endif  // HPP_CNM_CORE_TOPOLOGY_RING_RING_NODE_HPP
