#ifndef HPP_CNM_CORE_CONNECTION_CONNECTION_HPP
#define HPP_CNM_CORE_CONNECTION_CONNECTION_HPP

#include <memory>
#include <mutex>
#include <type_traits>
#include <vector>

#include "cnm/connection/client_ctx.hpp"
#include "cnm/connection/internal/client_node.hpp"
#include "cnm/connection/internal/connection.hpp"
#include "cnm/connection/internal/connection_node.hpp"
#include "cnm/connection/internal/server_node.hpp"
#include "cnm/connection/server_ctx.hpp"
#include "cnm/topology/base/node.hpp"
#include "cnm/utils/one_time_builder.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

class Connection final : public Connections::Connection {
  enum class SendingDirection { ToClient, ToServer, None };

 public:
  Connection(size_t net_speed,
             const std::vector<std::shared_ptr<Cnm::Node>> nodes);

  ~Connection() override;

  void abort() override;

  bool isRequesting() const noexcept override;

  bool isServing() const noexcept override;

  bool isAborted() const noexcept override;

  std::unique_lock<std::mutex> makeLock() const noexcept override;

  size_t getSpeed() const noexcept override;

  void stopRequesting() override;

  void stopServing() override;

  constexpr std::vector<std::shared_ptr<Connections::ConnectionNode>>::iterator
  begin() {
    return nodes.begin();
  }

  constexpr std::vector<std::shared_ptr<Connections::ConnectionNode>>::iterator
  end() {
    return nodes.end();
  }

  result_t<ClientCtx> createClientContext();

  result_t<ServerCtx> createServerContext();

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
