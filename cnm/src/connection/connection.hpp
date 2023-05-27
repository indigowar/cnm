#ifndef HPP_CNM_LIB_CONNECTION_CONNECTION_HPP
#define HPP_CNM_LIB_CONNECTION_CONNECTION_HPP

#include <memory>

#include "communication/channels/channel.hpp"
#include "communication/message.hpp"

namespace Cnm::Connection {

namespace Exceptions {
// TODO: Implement ContextAlreadyCreated exception class.
class ContextAlreadyCreated : public std::exception {};
}  // namespace Exceptions

class RequesterCtx;
class ServerCtx;

// todo: add isClosed method for Connection class.

class Connection final {
 public:
  Connection(size_t net_speed,
             std::shared_ptr<Communication::Channel<Communication::Message>>&&
                 req_chan,
             std::shared_ptr<Communication::Channel<Communication::Message>>&&
                 res_chan);

  ~Connection();

  size_t getNetSpeed() const noexcept;
  void setSpeed(size_t);

  void abort();

  std::unique_ptr<RequesterCtx> makeRequester();
  std::unique_ptr<ServerCtx> makeServer();

 private:
  std::shared_ptr<Communication::Channel<Communication::Message>>
      request_channel_;
  std::shared_ptr<Communication::Channel<Communication::Message>>
      response_channel_;

  size_t net_speed_;

  bool has_requester_;
  bool has_server_;

  mutable std::shared_mutex mutex_;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_LIB_CONNECTION_CONNECTION_HPP
