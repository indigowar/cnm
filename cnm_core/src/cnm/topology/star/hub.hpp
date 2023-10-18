#ifndef HPP_CNM_CORE_TOPOLOGY_STAR_HUB_HPP
#define HPP_CNM_CORE_TOPOLOGY_STAR_HUB_HPP

#include <mutex>
#include <set>
#include <utility>

#include "cnm/machine/machine.hpp"
#include "cnm/topology/base/node.hpp"

namespace Cnm::Star {

class Node;

class Communicator;

class Star;

class Hub final : public Cnm::Node {
 public:
  explicit Hub(Star *, HostInfo);

  ~Hub() override = default;

  void start() override;

  std::vector<ConnectionInfo> getConnections() const noexcept override;

  void stop() override;

  void invoke() override;

  void freeze() override;

  Object::Status getStatus() const noexcept override;

  HostInfo getHostInfo() const noexcept override;

  void setHostInfo(HostInfo) override;

  bool isServing() const noexcept override;

  bool isBusy() const noexcept override;

  std::vector<std::shared_ptr<Cnm::Node>> getConnectedNodes()
      const noexcept override;

  void serve(std::unique_ptr<ServerContext> &&ptr) override;

  std::string_view getType() const noexcept override { return TYPE; }

  void attachConnectionNode(Connections::ConnectionNode *node) override;

  void detachConnectionNode(Connections::ConnectionNode *node) override;

  std::unique_ptr<Cnm::Communicator> createCommunicator();

  result_t<HostInfo> addMachine(std::unique_ptr<Machine> &&, HostInfo);

  result_t<bool> deleteMachine(HostInfo);

  std::set<std::shared_ptr<Node>>::iterator begin() { return nodes.begin(); }

  std::set<std::shared_ptr<Node>>::iterator end() { return nodes.end(); }

  static constexpr std::string_view TYPE = "Hub";

 private:
  void forEachNode(std::function<void(std::shared_ptr<Node> &)>);

  friend class Communicator;
  friend class Star;

  HostInfo host_info;

  Star *star;

  std::set<std::shared_ptr<Node>> nodes;

  std::set<Connections::ConnectionNode *> connection_nodes;

  Object::Status status;

  mutable std::mutex mutex;
};

}  // namespace Cnm::Star

#endif  // HPP_CNM_CORE_TOPOLOGY_STAR_HUB_HPP
