#ifndef HPP_CNM_CORE_CONNECTION_INTERNAL_CLIENT_NODE_HPP
#define HPP_CNM_CORE_CONNECTION_INTERNAL_CLIENT_NODE_HPP

#include <vector>

#include "cnm/connection/internal/connection_node.hpp"

namespace Cnm::Connections {

// ClientNode - is a node that's exist on client machine.
// It has a next node and all Message's that are send backward to this node goes
// in specific buffer.
class ClientNode final : public ConnectionNode {
 public:
  ClientNode(Connection&, std::shared_ptr<Node>, const Utils::SleepWrapper&);

  ~ClientNode() override;

  void setNextNode(std::shared_ptr<ConnectionNode>) override;

  std::shared_ptr<ConnectionNode> getNextNode() const noexcept override;

  void setPreviousNode(std::shared_ptr<ConnectionNode>) override;

  std::shared_ptr<ConnectionNode> getPreviousNode() const noexcept override;

  void sendForward(Message&&) override;

  void sendBackward(Message&&) override;

  void abort() override;

  const std::vector<Message>& getBuffer() const noexcept;

 private:
  void callAbort();

  std::shared_ptr<ConnectionNode> next;

  std::vector<Message> retrieved;
};

}  // namespace Cnm::Connections

#endif  // HPP_CNM_CORE_CONNECTION_INTERNAL_CLIENT_NODE_HPP
