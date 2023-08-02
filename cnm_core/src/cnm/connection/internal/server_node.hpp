#ifndef HPP_CNM_CORE_CONNECTION_INTERNAL_SERVER_NODE_HPP
#define HPP_CNM_CORE_CONNECTION_INTERNAL_SERVER_NODE_HPP

#include "cnm/connection/internal/connection_node.hpp"

namespace Cnm::Connections {

// ServerNode - is the end node of the connection.
class ServerNode final : public ConnectionNode {
 public:
  ServerNode(Connection& connection, std::shared_ptr<Node> node,
             const Utils::SleepWrapper& sw);

  ~ServerNode() override;

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

  std::shared_ptr<ConnectionNode> previous;

  std::vector<Message> retrieved;
};

}  // namespace Cnm::Connections

#endif  // HPP_CNM_CORE_CONNECTION_INTERNAL_SERVER_NODE_HPP
