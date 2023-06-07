#ifndef HPP_CNM_LIB_CONNECTION_CONNECTION_HPP
#define HPP_CNM_LIB_CONNECTION_CONNECTION_HPP

#include <condition_variable>
#include <memory>

#include "communication/channels/channel.hpp"
#include "communication/message.hpp"

namespace Cnm::Connection {

class RequesterCtx;
class ServerCtx;

class Connection {
 public:
  Connection(
      std::shared_ptr<Cnm::Communication::Channel<Communication::Message>>&&
          request,
      std::shared_ptr<Cnm::Communication::Channel<Communication::Message>>&&
          response);

  ~Connection();

  size_t getNetSpeed() const noexcept;

  void setSpeed(size_t);

  void abort();

  std::unique_ptr<RequesterCtx> makeRequester();
  std::unique_ptr<ServerCtx> makeServer();

  // Connection is in the state Established, means that both
  // participants(requester and server) were found and connected to each
  // other. on the stage of `Established` connection is trasporting requests
  // and can not transport responses.
  bool isEstablished() const noexcept;

  // Connection is in the state Serving, means that both participants were
  // found and connected to each other. on this stage connection is sending
  // only responses and request channel is already closed.
  bool isServing() const noexcept;

  // Connection is in the state of Closed, when both channels are closed. No
  // message can't be transferred.
  bool isClosed() const noexcept;

  std::shared_ptr<Communication::Channel<Communication::Message>>
  getRequestChannel();
  std::shared_ptr<Communication::Channel<Communication::Message>>
  getResponseChannel();

  int getId() const noexcept { return id_; }

  void waitForEstablished();

  void waitForServing();

  void waitForClosed();

  void stopRequesting();
  void stopServing();

 private:
  bool stateIsCreated() const noexcept;
  bool stateIsEstablished() const noexcept;
  bool stateIsServing() const noexcept;
  bool stateIsClosed() const noexcept;

  const int id_;
  size_t net_speed_;
  bool has_requester_;
  bool has_server_;

  int requester_;
  int server_;

  std::shared_ptr<Communication::Channel<Communication::Message>> request_chan_;
  std::shared_ptr<Communication::Channel<Communication::Message>>
      response_chan_;

  mutable std::mutex mutex_;
  std::condition_variable cond_var_;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_LIB_CONNECTION_CONNECTION_HPP
