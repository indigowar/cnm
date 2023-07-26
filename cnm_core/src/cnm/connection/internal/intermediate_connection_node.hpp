#ifndef HPP_CNM_CORE_CONNECTION_INTERNAL_INTERMEDIATE_CONNECTION_NODE_HPP
#define HPP_CNM_CORE_CONNECTION_INTERNAL_INTERMEDIATE_CONNECTION_NODE_HPP

#include <memory>
#include <mutex>

#include "cnm/connection/internal/base_connection_node.hpp"
#include "cnm/connection/internal/connection_chain.hpp"
#include "cnm/core/message.hpp"

namespace Cnm::Connection {

class IntermediateConnectionNode final : public BaseConnectionNode {
 public:
  explicit IntermediateConnectionNode(
      ConnectionChain& connection, std::shared_ptr<_Node> node,
      std::shared_ptr<BaseConnectionNode> prev = nullptr,
      std::shared_ptr<BaseConnectionNode> next = nullptr);

  ~IntermediateConnectionNode();

  void setNextNode(std::shared_ptr<BaseConnectionNode>) override;

  std::shared_ptr<BaseConnectionNode> getNextNode() const noexcept override;

  void setPreviousNode(std::shared_ptr<BaseConnectionNode>) override;

  std::shared_ptr<BaseConnectionNode> getPreviousNode() const noexcept override;

  void sendForward(MessageBatch&&) override;

  void sendBackward(MessageBatch&&) override;

  void abort() override;

 private:
  std::shared_ptr<BaseConnectionNode> previous;
  std::shared_ptr<BaseConnectionNode> next;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_CORE_CONNECTION_INTERNAL_INTERMEDIATE_CONNECTION_NODE_HPP
