#ifndef HPP_CNM_CORE_TOPOLOGY_STAR_HUB_HPP
#define HPP_CNM_CORE_TOPOLOGY_STAR_HUB_HPP

#include <set>
#include <utility>

#include "cnm/machine/machine.hpp"
#include "cnm/topology/base/node.hpp"

namespace Cnm::Star {

class Node;

class Communicator;

class Hub final : public Cnm::Node {
 public:
  explicit Hub(HostInfo);

  ~Hub() override = default;

  void start() override;

  std::vector<ConnectionInfo> getConnections() const noexcept override;

  void stop() override;

  void invoke() override;

  void freeze() override;

  HostInfo getHostInfo() const noexcept override;

  void setHostInfo(HostInfo) override;

  bool isServing() const noexcept override;

  bool isBusy() const noexcept override;

  std::vector<std::shared_ptr<Cnm::Node>> getConnectedNodes()
      const noexcept override;

  void serve(std::unique_ptr<ServerContext> &&ptr) override;

  std::string_view getType() const noexcept override;

  void attachConnectionNode(Connections::ConnectionNode *node) override;

  void detachConnectionNode(Connections::ConnectionNode *node) override;

  std::unique_ptr<Communicator> createCommunicator();

  result_t<HostInfo> addMachine(std::unique_ptr<Machine> &&, HostInfo);

  result_t<bool> deleteMachine(HostInfo);

  std::set<std::shared_ptr<Star::Node>>::iterator begin() {
    return nodes.begin();
  }

  std::set<std::shared_ptr<Star::Node>>::iterator end() { return nodes.end(); }

 private:
  friend class Communicator;

  HostInfo host_info;

  std::set<std::shared_ptr<Star::Node>> nodes;

  std::set<Connections::ConnectionNode *> connection_nodes;

  mutable std::mutex mutex;
};

}  // namespace Cnm::Star

#endif  // HPP_CNM_CORE_TOPOLOGY_STAR_HUB_HPP
