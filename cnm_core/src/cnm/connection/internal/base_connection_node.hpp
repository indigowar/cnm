#ifndef HPP_CNM_CORE_CONNECTION_INTERNAL_BASE_CONNECTION_NODE_HPP
#define HPP_CNM_CORE_CONNECTION_INTERNAL_BASE_CONNECTION_NODE_HPP

#include <memory>
#include <mutex>

#include "cnm/connection/internal/connection_chain.hpp"
#include "cnm/core/message.hpp"

namespace Cnm::Connection {

class _Node {};

class BaseConnectionNode {
 public:
  BaseConnectionNode(ConnectionChain& connection, std::shared_ptr<_Node> node)
      : owner{connection}, node{std::move(node)} {}

  virtual ~BaseConnectionNode() = default;

  virtual void setNextNode(std::shared_ptr<BaseConnectionNode>) = 0;
  virtual std::shared_ptr<BaseConnectionNode> getNextNode() const noexcept = 0;

  virtual void setPreviousNode(std::shared_ptr<BaseConnectionNode>) = 0;
  virtual std::shared_ptr<BaseConnectionNode> getPreviousNode()
      const noexcept = 0;

  virtual void sendForward(MessageBatch&&) = 0;
  virtual void sendBackward(MessageBatch&&) = 0;

  virtual void abort() = 0;

  std::unique_lock<std::mutex> makeLock() const {
    return std::unique_lock(mutex);
  }

  virtual std::shared_ptr<_Node> getNetworkNode() const noexcept {
    return node;
  }

  void setOwner(ConnectionChain& new_owner) { owner = new_owner; }

  ConnectionChain& getOwner() const noexcept {
    auto lock = makeLock();
    return owner;
  }

 private:
  ConnectionChain& owner;

  std::shared_ptr<_Node> node;

  mutable std::mutex mutex;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_CORE_CONNECTION_INTERNAL_BASE_CONNECTION_NODE_HPP
