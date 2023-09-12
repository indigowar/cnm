#ifndef HPP_CNM_CORE_TOPOLOGY_STAR_STAR_NODE_HPP
#define HPP_CNM_CORE_TOPOLOGY_STAR_STAR_NODE_HPP

#include <set>

#include "cnm/machine/communicator.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/topology/base/node.hpp"

namespace Cnm {

class Hub;

class StarNode final : public Node {
 public:
  StarNode(HostInfo, std::unique_ptr<Machine>&&, const std::shared_ptr<Hub>&);

  ~StarNode() override;

  [[nodiscard]] HostInfo getHostInfo() const noexcept override;

  void setHostInfo(HostInfo) override;

  std::vector<std::shared_ptr<Node>> getConnectedNodes()
      const noexcept override;

  bool isServing() const noexcept override;

  bool isBusy() const noexcept override;

  void start() override;
  void stop() override;
  void invoke() override;
  void freeze() override;

  void serve(std::unique_ptr<ServerContext>&&) override;

  std::string_view getType() const noexcept override;

  void attachConnectionNode(Connections::ConnectionNode* ptr) override;

  void detachConnectionNode(Connections::ConnectionNode* ptr) override;

  std::vector<ConnectionInfo> getConnections() const noexcept override;

 private:
  std::shared_ptr<Hub> hub;

  std::unique_ptr<Machine> machine;

  mutable std::mutex mutex;

  std::set<Cnm::Connections::ConnectionNode*> connection_nodes;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_STAR_STAR_NODE_HPP
