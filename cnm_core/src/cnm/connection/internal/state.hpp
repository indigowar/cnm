#pragma once

#include <iterator>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include "cnm/core/message.hpp"

namespace Cnm::Connection::Internal {

class TopologyNode {};

class BaseConnectionNode {
 public:
  virtual void setNextNode(std::shared_ptr<BaseConnectionNode>) = 0;
  virtual std::shared_ptr<BaseConnectionNode> getNextNode() const noexcept = 0;

  virtual void setPreviousNode(std::shared_ptr<BaseConnectionNode>) = 0;
  virtual std::shared_ptr<BaseConnectionNode> getPreviousNode()
      const noexcept = 0;

  virtual void sendForward(MessageBatch&&) = 0;
  virtual void sendBackward(MessageBatch&&) = 0;

  virtual void abort() = 0;

  virtual std::unique_lock<std::mutex> makeLock() = 0;
};

class ClientNode : public BaseConnectionNode {};
class ServerNode : public BaseConnectionNode {};

class IntermediateConnectionNode : public BaseConnectionNode {
 public:
  explicit IntermediateConnectionNode(
      std::shared_ptr<TopologyNode> node,
      std::shared_ptr<BaseConnectionNode> prev = nullptr,
      std::shared_ptr<BaseConnectionNode> next = nullptr)
      : node{std::move(node)},
        previous{std::move(prev)},
        next{std::move(next)} {}

  ~IntermediateConnectionNode() { abort(); }

  void setNextNode(std::shared_ptr<BaseConnectionNode> next) override {
    auto lock = makeLock();
    this->next = next;
  }

  std::shared_ptr<BaseConnectionNode> getNextNode() const noexcept override {
    return next;
  }

  std::shared_ptr<BaseConnectionNode> getPreviousNode()
      const noexcept override {
    return previous;
  }

  void setPreviousNode(std::shared_ptr<BaseConnectionNode> previous) override {
    auto lock = makeLock();
    this->previous = previous;
  }

  void sendForward(MessageBatch&& msg) override {
    auto lock = makeLock();
    if (next) next->sendForward(std::move(msg));
  }

  void sendBackward(MessageBatch&& msg) override {
    auto lock = makeLock();
    if (previous) previous->sendBackward(std::move(msg));
  }

  void abort() override {
    auto lock = makeLock();
    if (previous) previous = nullptr;
    if (next) next = nullptr;
  }

  std::unique_lock<std::mutex> makeLock() override {
    return std::unique_lock(mutex);
  }

 private:
  std::shared_ptr<TopologyNode> node;

  std::shared_ptr<BaseConnectionNode> previous;
  std::shared_ptr<BaseConnectionNode> next;

  std::mutex mutex;
};

class Chain {
 public:
  explicit Chain() = default;

  void addNode(std::shared_ptr<TopologyNode> target) {
    std::unique_lock chain_lock(mutex);
    auto node = std::make_shared<IntermediateConnectionNode>(std::move(target));
    if (head == nullptr) {
      head = tail = node;
    } else {
      tail->setNextNode(node);
    }
    node->setNextNode(tail);
    tail = node;
  }

 private:
  std::shared_ptr<BaseConnectionNode> head;
  std::shared_ptr<BaseConnectionNode> tail;

  std::mutex mutex;
};

}  // namespace Cnm::Connection::Internal
