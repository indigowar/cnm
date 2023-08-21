#ifndef HPP_CNM_CORE_TOPOLOGY_RING_RING_NODE_HPP
#define HPP_CNM_CORE_TOPOLOGY_RING_RING_NODE_HPP

#include <memory>
#include <mutex>

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

 private:
  std::shared_ptr<RingNode> previous_node;
  std::shared_ptr<RingNode> next_node;

  std::unique_ptr<Machine> machine;

  mutable std::mutex mutex;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_RING_RING_NODE_HPP
