#ifndef HPP_CNM_LIB_CONNECTION_CHANNEL_HPP
#define HPP_CNM_LIB_CONNECTION_CHANNEL_HPP

#include "communication/channels/channel.hpp"
#include "communication/message.hpp"
#include "connection/connection.hpp"

namespace Cnm::Connection {

/**
 * A proxy that has abillity to send signals to the Connection Class
 */

class Channel
    : public Cnm::Communication::WritableChannel<Communication::Message>,
      public Cnm::Communication::ReadableChannel<Communication::Message> {
 public:
  Channel(
      Connection& con,
      std::unique_ptr<Cnm::Communication::Channel<Communication::Message>> impl)
      : connection_{con}, impl_{std::move(impl)} {}

  [[nodiscard]] bool isBuffered() const noexcept override {
    return impl_->isBuffered();
  }

  [[nodiscard]] bool isClosed() const noexcept override {
    return impl_->isClosed();
  }

  [[nodiscard]] size_t getLimit() const override { return impl_->getLimit(); }

  [[nodiscard]] size_t getAmountOfContained() const noexcept override {
    return impl_->getAmountOfContained();
  }

  void close() { impl_->close(); }

  WritableChannel<Communication::Message>& operator<<(
      Communication::Message value) override {
    *impl_ << value;
    return *this;
  }

  ReadableChannel<Communication::Message>& operator>>(
      Communication::Message& value) override {
    *impl_ >> value;
    return *this;
  }

  ReadableChannel<Communication::Message>& operator>>(
      std::future<Communication::Message>& value) override {
    *impl_ >> value;
    return *this;
  }

 private:
  std::unique_ptr<Cnm::Communication::Channel<Communication::Message>> impl_;
  Cnm::Connection::Connection& connection_;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_LIB_CONNECTION_CHANNEL_HPP
