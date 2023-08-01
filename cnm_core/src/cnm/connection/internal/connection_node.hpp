#ifndef HPP_CNM_CORE_CONNECTION_INTERNAL_CONNECTION_NODE_HPP
#define HPP_CNM_CORE_CONNECTION_INTERNAL_CONNECTION_NODE_HPP

#include <memory>
#include <mutex>

#include "cnm/connection/internal/connection.hpp"
#include "cnm/core/message.hpp"
#include "cnm/utils/sleep_wrapper.hpp"

namespace Cnm::Connections {

// ConnectionNode is base class for connection's nodes.
class ConnectionNode {
 public:
  ConnectionNode(Connection& connection, std::shared_ptr<_Node> node,
                 const Utils::SleepWrapper& sw)
      : owner{connection}, node{std::move(node)}, sleepWrapper(sw) {}

  virtual ~ConnectionNode() = default;

  virtual void setNextNode(std::shared_ptr<ConnectionNode>) = 0;
  virtual std::shared_ptr<ConnectionNode> getNextNode() const noexcept = 0;

  virtual void setPreviousNode(std::shared_ptr<ConnectionNode>) = 0;
  virtual std::shared_ptr<ConnectionNode> getPreviousNode() const noexcept = 0;

  virtual void sendForward(Message&&) = 0;
  virtual void sendBackward(Message&&) = 0;

  virtual void abort() = 0;

  std::unique_lock<std::mutex> makeLock() const {
    return std::unique_lock(mutex);
  }

  std::shared_ptr<_Node> getNetworkNode() const noexcept { return node; }

  void setOwner(Connection& new_owner) { owner = new_owner; }

  Connection& getOwner() const noexcept { return owner; }

  const Utils::SleepWrapper& getSleepWrapper() const noexcept {
    return sleepWrapper;
  }

 private:
  Connection& owner;

  std::shared_ptr<_Node> node;

  const Utils::SleepWrapper sleepWrapper;

  mutable std::mutex mutex;
};

}  // namespace Cnm::Connections

#endif  // HPP_CNM_CORE_CONNECTION_INTERNAL_CONNECTION_NODE_HPP
