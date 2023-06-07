#include "connection.hpp"

#include <random>

#include "communication/channels/channel.hpp"
#include "connection/exceptions.hpp"
#include "connection/requester_ctx.hpp"
#include "connection/server_ctx.hpp"

using chan_t =
    std::shared_ptr<Cnm::Communication::Channel<Cnm::Communication::Message>>;

namespace {

static int generateRandomInt() {
  std::random_device device;
  std::mt19937 rng(device());
  std::uniform_int_distribution<int> distribution(1, 10'000'000);
  return distribution(rng);
}

}  // namespace

namespace Cnm::Connection {

Connection::Connection(chan_t&& request_chan, chan_t&& response_chan)
    : id_{::generateRandomInt()},
      request_chan_{request_chan},
      response_chan_{response_chan} {}

Connection::~Connection() {}

size_t Connection::getNetSpeed() const noexcept { return net_speed_; }

void Connection::setSpeed(size_t new_value) { net_speed_ = new_value; }

void Connection::abort() {
  std::unique_lock lock(mutex_);

  if (has_requester_ && !request_chan_->isClosed()) {
    request_chan_->close();
  }

  if (!response_chan_->isClosed()) {
    response_chan_->close();
  }

  cond_var_.notify_all();
}

std::unique_ptr<RequesterCtx> Connection::makeRequester() {
  std::unique_lock lock(mutex_);
  if (has_requester_) {
    throw Cnm::Connection::Exceptions::ContextAlreadyCreated(getId(),
                                                             "requester");
  }
  has_requester_ = true;
  return std::make_unique<RequesterCtx>(*this, this->requester_);
}

std::unique_ptr<ServerCtx> Connection::makeServer() {
  std::unique_lock lock(mutex_);
  if (!stateIsCreated()) {
    throw Cnm::Connection::Exceptions::ConnectionStateError(
        getId(), "makeServer requires connection to be created state");
  }

  if (has_server_) {
    throw Cnm::Connection::Exceptions::ContextAlreadyCreated(getId(), "server");
  }

  if (stateIsEstablished()) {
    cond_var_.notify_all();
  }

  has_server_ = true;
  return std::make_unique<ServerCtx>(*this, server_);
}

chan_t Connection::getRequestChannel() { return request_chan_; }
chan_t Connection::getResponseChannel() { return response_chan_; }

void Connection::waitForEstablished() {
  std::unique_lock lock(mutex_);

  if (stateIsEstablished()) {
    return;
  }

  if (!stateIsCreated()) {
    throw Cnm::Connection::Exceptions::ConnectionStateError(
        getId(), "this connection cannot change to `Established` state");
  }

  cond_var_.wait(lock, [this] { return stateIsEstablished(); });

  if (!stateIsEstablished()) {
    throw Cnm::Connection::Exceptions::WaiterGotDifferentState(
        getId(), "Established", "Another one");
  }
}

void Connection::waitForServing() {
  std::unique_lock lock(mutex_);

  if (stateIsServing()) {
    return;
  }

  if (!stateIsEstablished()) {
    throw Cnm::Connection::Exceptions::ConnectionStateError(
        getId(), "this connection cannot change to `Established` state");
  }

  cond_var_.wait(lock, [this] { return stateIsServing(); });

  if (!stateIsServing()) {
    throw Cnm::Connection::Exceptions::WaiterGotDifferentState(
        getId(), "Serving", "Another one");
  }
}

void Connection::waitForClosed() {
  std::unique_lock lock(mutex_);

  if (stateIsClosed()) {
    return;
  }

  cond_var_.wait(lock, [this] { return stateIsClosed(); });

  if (!stateIsServing()) {
    throw Cnm::Connection::Exceptions::WaiterGotDifferentState(
        getId(), "Closed", "Another one");
  }
}

void Connection::stopRequesting() {
  std::unique_lock lock(mutex_);
  if (!stateIsEstablished()) {
    throw Cnm::Connection::Exceptions::ConnectionStateError(
        getId(), "this connection is not in `Established` state");
  }

  request_chan_->close();
  cond_var_.notify_all();
}

void Connection::stopServing() {
  std::unique_lock lock(mutex_);
  if (!stateIsServing()) {
    throw Cnm::Connection::Exceptions::ConnectionStateError(
        getId(), "this connection is not in `Serving` state");
  }

  response_chan_->close();
  cond_var_.notify_all();
}

}  // namespace Cnm::Connection
