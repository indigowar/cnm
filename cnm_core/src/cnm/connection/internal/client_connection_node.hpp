#ifndef HPP_CNM_CORE_CONNECTION_INTERNAL_CLIENT_CONNECTION_NODE_HPP
#define HPP_CNM_CORE_CONNECTION_INTERNAL_CLIENT_CONNECTION_NODE_HPP

#include <stdexcept>

#include "cnm/connection/internal/base_connection_node.hpp"
#include "cnm/connection/internal/connection_chain.hpp"

namespace Cnm::Connection {

class ClientConnectionNode final : public BaseConnectionNode {
 public:
  ClientConnectionNode(ConnectionChain& connection, std::shared_ptr<_Node> node,
                       std::shared_ptr<BaseConnectionNode> next);

  ~ClientConnectionNode();

  void setNextNode(std::shared_ptr<BaseConnectionNode>) override;

  std::shared_ptr<BaseConnectionNode> getNextNode() const noexcept override;

  void setPreviousNode(std::shared_ptr<BaseConnectionNode>) override;
  std::shared_ptr<BaseConnectionNode> getPreviousNode() const noexcept override;

  void sendForward(MessageBatch&&) override;
  void sendBackward(MessageBatch&&) override;

  void abort() override;

 private:
  std::shared_ptr<BaseConnectionNode> next;

  mutable std::mutex mutex;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_CORE_CONNECTION_INTERNAL_CLIENT_CONNECTION_NODE_HPP
