#ifndef HPP_CNM_CORE_CONNECTION_INTERNAL_CLIENT_NODE_HPP
#define HPP_CNM_CORE_CONNECTION_INTERNAL_CLIENT_NODE_HPP

#include <stdexcept>

#include "cnm/connection/internal/connection.hpp"
#include "cnm/connection/internal/connection_node.hpp"

namespace Cnm::Connections {

class ClientNode final : public ConnectionNode {
 public:
  ClientNode(Connection& connection, std::shared_ptr<_Node> node,
             std::shared_ptr<ConnectionNode> next);

  ~ClientNode();

  void setNextNode(std::shared_ptr<ConnectionNode>) override;

  std::shared_ptr<ConnectionNode> getNextNode() const noexcept override;

  void setPreviousNode(std::shared_ptr<ConnectionNode>) override;
  std::shared_ptr<ConnectionNode> getPreviousNode() const noexcept override;

  void sendForward(Message&&) override;
  void sendBackward(Message&&) override;

  void abort() override;

 private:
  std::shared_ptr<ConnectionNode> next;

  mutable std::mutex mutex;
};

}  // namespace Cnm::Connections

#endif  // HPP_CNM_CORE_CONNECTION_INTERNAL_CLIENT_NODE_HPP
