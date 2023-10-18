#ifndef HPP_CNM_CORE_CONNECTION_CLIENT_CTX_HPP
#define HPP_CNM_CORE_CONNECTION_CLIENT_CTX_HPP

#include <future>
#include <memory>

#include "cnm/connection/internal/client_node.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

class Connection;

// ClientContext an object to interact with connection as a client(requester).
class ClientContext final {
 public:
  ClientContext(std::shared_ptr<Connection>, Connections::ClientT);

  ~ClientContext();

  // getFutureUntilAccepted - return a future, that will be filled with true,
  // when the connection will get to the state Accepted, or will return an
  // error.
  std::future<result_t<bool>> getFutureUntilAccepted();

  // waitUntilAccepted - returns the result of switching the state of
  // connection, it's wrapper on getFutureUntilAccepted that handles the future.
  result_t<bool> waitUntilAccepted();

  // request - sends the request(argument) to the server node using connection.
  // returns the result of operation.
  result_t<bool> request(MessageBatch&&);

  // getFutureResponse - returns a future that will be filled with response from
  // the server.
  std::future<result_t<MessageBatch>> getFutureResponse();

  // getResponse() - is a wrapper on getFutureResponse, and it's return the
  // response from the server, it handles the work with the future inside.
  result_t<MessageBatch> getResponse();

  // abort() - terminate the connection immediately, because of some error.
  void abort();

  ClientContext(const ClientContext&) = delete;
  ClientContext& operator=(const ClientContext&) = delete;

  ClientContext(ClientContext&&) noexcept;
  ClientContext& operator=(ClientContext&&) noexcept;

 private:
  std::shared_ptr<Connection> connection;

  Connections::ClientT client_node;
};

using ClientCtx = std::unique_ptr<ClientContext>;
}  // namespace Cnm

#endif  // HPP_CNM_CORE_CONNECTION_CLIENT_CTX_HPP