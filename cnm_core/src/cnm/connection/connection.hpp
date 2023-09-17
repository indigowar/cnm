#ifndef HPP_CNM_CORE_CONNECTION_CONNECTION_HPP
#define HPP_CNM_CORE_CONNECTION_CONNECTION_HPP

#include <spdlog/spdlog.h>

#include <memory>
#include <mutex>
#include <type_traits>
#include <vector>

#include "cnm/connection/client_ctx.hpp"
#include "cnm/connection/internal/client_node.hpp"
#include "cnm/connection/internal/connection.hpp"
#include "cnm/connection/internal/connection_node.hpp"
#include "cnm/connection/internal/intermediate_node.hpp"
#include "cnm/connection/internal/server_node.hpp"
#include "cnm/connection/server_ctx.hpp"
#include "cnm/utils/one_time_builder.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

class Connection final : public Connections::Connection {
  enum class SendingDirection { ToClient, ToServer, None };

 public:
  template <typename Iter>
  Connection(size_t net_speed, Iter begin, Iter end) {
    if (std::distance(begin, end) < 2) {
      throw std::runtime_error(
          "can't create connection with less than 2 nodes.");
    }

    sleep_wrapper = std::move(std::make_shared<Utils::SleepWrapper>());

    client_node =
        std::make_shared<Connections::ClientNode>(*this, *begin, sleep_wrapper);

    std::shared_ptr<Connections::ConnectionNode> prev = client_node;
    Iter prev_node = begin;
    for (auto it = begin + 1; std::distance(it, end) > 1; it++) {
      auto intermediate = std::make_shared<Connections::IntermediateNode>(
          *this, *it, sleep_wrapper, prev);
      prev->setNextNode(intermediate);
      prev = intermediate;
      prev_node = it;
    }

    auto it = prev_node + 1;
    server_node =
        std::make_shared<Connections::ServerNode>(*this, *it, sleep_wrapper);
    server_node->setPreviousNode(prev);
    prev->setNextNode(server_node);

    speed = net_speed;
    is_aborted = false;
    current_direction = SendingDirection::ToServer;
  }

  ~Connection() override;

  void abort() override;

  bool isRequesting() const noexcept override;

  bool isServing() const noexcept override;

  bool isAborted() const noexcept override;

  std::unique_lock<std::mutex> makeLock() const noexcept override;

  size_t getSpeed() const noexcept override;

  void stopRequesting() override;

  void stopServing() override;

  std::vector<std::shared_ptr<Connections::ConnectionNode>>::iterator begin() {
    return nodes.begin();
  }

  std::vector<std::shared_ptr<Connections::ConnectionNode>>::iterator end() {
    return nodes.end();
  }

  result_t<ClientCtx> createClientContext();

  result_t<ServerCtx> createServerContext();

  HostInfo getClientHostInfo() const noexcept override;

  HostInfo getServerHostInfo() const noexcept override;

 private:
  void callAbort();

  std::vector<std::shared_ptr<Connections::ConnectionNode>> nodes;

  std::shared_ptr<Connections::ClientNode> client_node;
  std::shared_ptr<Connections::ServerNode> server_node;

  std::shared_ptr<Utils::SleepWrapper> sleep_wrapper;

  size_t speed;

  bool is_aborted;
  SendingDirection current_direction;

  Utils::OneTimeBuilder<ClientContext> client_ctx_builder;
  Utils::OneTimeBuilder<ServerContext> server_ctx_builder;

  mutable std::mutex mutex;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_CONNECTION_CONNECTION_HPP
