#ifndef HPP_CNM_CORE_CONNECTION_INTERNAL_INTERMEDIATE_NODE_HPP
#define HPP_CNM_CORE_CONNECTION_INTERNAL_INTERMEDIATE_NODE_HPP

#include <memory>
#include <mutex>

#include "cnm/connection/internal/connection.hpp"
#include "cnm/connection/internal/connection_node.hpp"
#include "cnm/core/message.hpp"
#include "cnm/utils/sleep_wrapper.hpp"

namespace Cnm::Connections {

// IntermediateNode is the node, which exist between two other nodes(it can be
// other IntermediateNodes or Client/Server nodes).
class IntermediateNode final : public ConnectionNode {
 public:
  IntermediateNode(Connection& connection, std::shared_ptr<Node> node,
                   const Utils::SleepWrapper& sleeper,
                   std::shared_ptr<ConnectionNode> prev = nullptr,
                   std::shared_ptr<ConnectionNode> next = nullptr);

  ~IntermediateNode() override;

  void setNextNode(std::shared_ptr<ConnectionNode>) override;

  std::shared_ptr<ConnectionNode> getNextNode() const noexcept override;

  void setPreviousNode(std::shared_ptr<ConnectionNode>) override;

  std::shared_ptr<ConnectionNode> getPreviousNode() const noexcept override;

  void sendForward(Message&&) override;

  void sendBackward(Message&&) override;

  void abort() override;

 private:
  void callAbort();

  std::shared_ptr<ConnectionNode> previous;
  std::shared_ptr<ConnectionNode> next;
};

}  // namespace Cnm::Connections

#endif  // HPP_CNM_CORE_CONNECTION_INTERNAL_INTERMEDIATE_NODE_HPP
