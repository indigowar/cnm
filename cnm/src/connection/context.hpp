#ifndef HPP_CNM_LIB_CONNECTION_CONNECTION_CONTEXT_HPP
#define HPP_CNM_LIB_CONNECTION_CONNECTION_CONTEXT_HPP

#include <cstddef>

namespace Cnm::Connection {

// TODO: Implement Context class.

class Context {
 public:
  explicit Context(Connection& con) : connection_(con) {}

  virtual ~Context() = default;

  [[nodiscard]] bool isEstablished() const noexcept {}

  [[nodiscard]] bool isServing() const noexcept {}

  [[nodiscard]] bool isClosed() const noexcept;

  [[nodiscard]] virtual size_t getNetworkSpeed() const noexcept;

  [[nodiscard]] size_t getRequestLimit() const noexcept;
  [[nodiscard]] size_t getResponseLimit() const noexcept;

  void abort();

  void waitForEstablished();
  void waitForServing();
  void waitForEnded();

 protected:
  Connection& connection_;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_LIB_CONNECTION_CONNECTION_CONTEXT_HPP
