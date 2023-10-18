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

// ServerContext - is an object to interact with Connection as a Server.
class ServerContext final {
 public:
  ServerContext(std::shared_ptr<Connection>, Connections::ServerT);

  ~ServerContext();

  // accept() - accept the request, that was sent by client.
  void accept();

  // getFutureRequest() - returns a future that will be filled with request.
  std::future<result_t<MessageBatch>> getFutureRequest();

  // acceptAndGetFutureRequest() - is a wrapper, that uses accept() and
  // getFutureRequest()
  std::future<result_t<MessageBatch>> acceptAndGetFutureRequest();

  // getRequest - is a wrapper on getFutureRequest that handles the future and
  // returns the request.
  result_t<MessageBatch> getRequest();

  // acceptAndGetRequest - is a wrapper, that uses accept() and getRequest()
  result_t<MessageBatch> acceptAndGetRequest();

  // abort() - abort the connection
  void abort();

  // respond() - sends a response to the client
  result_t<bool> respond(const MessageBatch&);

  ServerContext(const ServerContext&) = delete;
  ServerContext& operator=(const ServerContext&) = delete;

  ServerContext(ServerContext&&) noexcept;
  ServerContext& operator=(ServerContext&&) noexcept;

 private:
  std::shared_ptr<Connection> connection;
  Connections::ServerT server_node;
};

using ServerCtx = std::unique_ptr<ServerContext>;

}  // namespace Cnm

#endif  // HPP_CNM_CORE_CONNECTION_SERVER_CTX_HPP
