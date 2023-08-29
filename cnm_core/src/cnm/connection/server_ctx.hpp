#ifndef HPP_CNM_CORE_CONNECTION_SERVER_CTX_HPP
#define HPP_CNM_CORE_CONNECTION_SERVER_CTX_HPP

#include <condition_variable>
#include <future>
#include <memory>

#include "cnm/connection/internal/server_node.hpp"
#include "cnm/core/message.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

class Connection;

class ServerContext final {
 public:
  ServerContext(Connection*, std::shared_ptr<Connections::ServerNode>);

  ~ServerContext() = default;

  ServerContext(const ServerContext&) = delete;
  ServerContext& operator=(const ServerContext&) = delete;

  ServerContext(ServerContext&&) noexcept;
  ServerContext& operator=(ServerContext&&) noexcept;

  std::future<result_t<MessageBatch>> acceptRequest();

  void abort();

  void sendResponse(MessageBatch&&);

 private:
  Connection* connection;
  std::shared_ptr<Connections::ServerNode> server_node;

  std::condition_variable cond_var;
};

using ServerCtx = std::unique_ptr<ServerContext>;

}  // namespace Cnm

#endif  // HPP_CNM_CORE_CONNECTION_SERVER_CTX_HPP
