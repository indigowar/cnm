#include "connection.hpp"

#include "connection/requester_ctx.hpp"
#include "connection/server_ctx.hpp"

using Cnm::Communication::Channel;
using Cnm::Communication::Message;
using Cnm::Connection::RequesterCtx;
using Cnm::Connection::ServerCtx;
using Cnm::Connection::Exceptions::ContextAlreadyCreated;

// enum class ConStates {
//   Created,      // created, but the server is not found
//   Established,  // the server is established, but the request isn't  sent
//   yet. Serving,      // the request is sent and waits for response Ended, //
//   exchange of information is ended
// };

Cnm::Connection::Connection::Connection(
    size_t net_speed, std::shared_ptr<Channel<Message>>&& req_chan,
    std::shared_ptr<Channel<Message>>&& res_chan)
    : request_channel_(std::move(req_chan)),
      response_channel_(std::move(res_chan)),
      net_speed_(net_speed),
      has_requester_{},
      has_server_{} {}

Cnm::Connection::Connection::~Connection() {
  // todo: implement Connection::~Connection()
}

size_t Cnm::Connection::Connection::getNetSpeed() const noexcept {
  std::shared_lock lock(mutex_);
  return net_speed_;
}

void Cnm::Connection::Connection::setSpeed(size_t speed) {
  std::unique_lock lock(mutex_);
  net_speed_ = speed;
}

std::unique_ptr<RequesterCtx>
Cnm::Connection::Connection::Connection::makeRequester() {
  std::unique_lock lock(mutex_);
  if (has_requester_) {
    throw ContextAlreadyCreated();
  }
  has_requester_ = true;
  return std::make_unique<RequesterCtx>(*this);
}

std::unique_ptr<ServerCtx>
Cnm::Connection::Connection::Connection::makeServer() {
  std::unique_lock lock(mutex_);
  if (has_server_) {
    throw ContextAlreadyCreated();
  }
  has_server_ = true;
  return std::make_unique<ServerCtx>(*this);
}

void Cnm::Connection::Connection::abort() {
  // TODO: implement Cnm::Connection::Connection::abort()
}
