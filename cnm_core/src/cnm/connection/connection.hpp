#ifndef HPP_CNM_CORE_CONNECTION_CONNECTION_HPP
#define HPP_CNM_CORE_CONNECTION_CONNECTION_HPP

#include <condition_variable>
#include <vector>

#include "cnm/connection/client_ctx.hpp"
#include "cnm/connection/internal/client_node.hpp"
#include "cnm/connection/internal/connection.hpp"
#include "cnm/connection/internal/server_node.hpp"
#include "cnm/connection/server_ctx.hpp"
#include "cnm/topology/base/node.hpp"

namespace Cnm {

// Connection - an object that rules over the communication between nodes.
// It implements Connections::Connection in order to communicate with its own
// connection  nodes.
class Connection final : public Connections::Connection {
  enum class Status { Created, Requesting, Serving, Complete, Aborted };

 public:
  Connection(size_t net_speed, const std::vector<std::shared_ptr<Node>>& nodes);

  void abort() override;

  [[nodiscard]] bool isRequesting() const noexcept override;

  [[nodiscard]] bool isServing() const noexcept override;

  [[nodiscard]] bool isAborted() const noexcept override;

  [[nodiscard]] std::unique_lock<std::mutex> makeLock() const noexcept override;

  [[nodiscard]] size_t getSpeed() const noexcept override;

  [[nodiscard]] HostInfo getClientHostInfo() const noexcept override;

  [[nodiscard]] HostInfo getServerHostInfo() const noexcept override;

  Connections::ClientT getClientNode();
  Connections::ServerT getServerNode();

  // signalAccept - sends a signal to connection, that it was accepted by the
  // server. Can be sent once and only when connection is Created.
  void signalAccept();

  // signalComplete - sends a signal to connection, that the server has sent
  // all the responses. Can be sent once and only when connection is Serving.
  void signalComplete();

  // signalStopRequest - sends a signal to connection, that the client has sent
  // all requests, and expects to start reading responses. Can be sent only once
  // and when connection is Requesting.
  void signalStopRequest();

  // waits til the connection comes in the stage of requesting, if connection
  // retrieves different state, returns an error.
  result_t<bool> waitForRequesting();

  // waits til the connection comes in the stage of serving, if connection
  // retrieves different state, returns an error.
  result_t<bool> waitForServing();

  // waits til the connection comes in the stage of complete, if connection
  // retrieves different state, returns an error.
  result_t<bool> waitForComplete();

 private:
  void changeStatus(Connection::Status);

  result_t<bool> waitForStatusChange(Status);

  void callAbort();

  Connections::ClientT client_node;
  Connections::ServerT server_node;

  // called_aborted is true, when the abort() has at least once been called.
  bool called_aborted;

  Status status;

  // net_speed - is amount of ms that required to pass a value between nodes.
  size_t net_speed;

  mutable std::condition_variable notifier;

  mutable std::mutex mutex;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_CONNECTION_CONNECTION_HPP
