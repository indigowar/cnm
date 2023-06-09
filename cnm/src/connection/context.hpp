#ifndef HPP_CNM_LIB_CONNECTION_CONNECTION_CONTEXT_HPP
#define HPP_CNM_LIB_CONNECTION_CONNECTION_CONTEXT_HPP

#include <spdlog/spdlog.h>

#include <cstddef>

#include "connection/connection.hpp"

namespace Cnm::Connection {

class Context {
 public:
  explicit Context(Connection& con) : connection_(con) {}

  virtual ~Context() = default;

  [[nodiscard]] virtual bool isEstablished() const noexcept {
    return connection_.isEstablished();
  }

  [[nodiscard]] virtual bool isServing() const noexcept {
    return connection_.isServing();
  }

  [[nodiscard]] virtual bool isClosed() const noexcept {
    return connection_.isClosed();
  }

  [[nodiscard]] virtual size_t getNetworkSpeed() const noexcept {
    return connection_.getNetSpeed();
  }

  [[nodiscard]] virtual size_t getRequestLimit() const noexcept {
    try {
      return connection_.getRequestChannel()->getLimit();
    } catch (const Communication::Exceptions::ChannelUnbuffered& ce) {
      spdlog::warn(
          "Unbuffered Channel is used as a request channel in the "
          "connection:%d",
          connection_.getId());

      return std::numeric_limits<size_t>::max();
    }
  }

  [[nodiscard]] virtual size_t getResponseLimit() const noexcept {
    try {
      return connection_.getResponseChannel()->getLimit();
    } catch (const Communication::Exceptions::ChannelUnbuffered& ce) {
      spdlog::warn(
          "Unbuffered Channel is used as a response channel in the connection: "
          "%d",
          connection_.getId());
      return std::numeric_limits<size_t>::max();
    }
  }

  virtual void abort() {
    spdlog::info("context called abort for ", connection_.getId());
    connection_.abort();
  }

  virtual void waitForEstablished() { connection_.waitForEstablished(); }

  virtual void waitForServing() { connection_.waitForServing(); }

  void waitForClosed() { connection_.waitForClosed(); }

 protected:
  Connection& connection_;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_LIB_CONNECTION_CONNECTION_CONTEXT_HPP
