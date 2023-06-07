#ifndef HPP_CNM_LIB_CONNECTION_EXCEPTIONS_HPP
#define HPP_CNM_LIB_CONNECTION_EXCEPTIONS_HPP

#include <exception>
#include <sstream>
#include <string_view>

namespace Cnm::Connection::Exceptions {

class ContextAlreadyCreated : public std::exception {
 public:
  ContextAlreadyCreated(int connection, std::string_view context)
      : connection_(connection), context_(context) {
    std::stringstream ss;
    ss << "Context " << context << " for connection " << connection_
       << " already was created!";
    msg_ = ss.str();
  }

  int connection() const noexcept { return connection_; }

  std::string_view context() const noexcept { return context_; }

  const char* what() const noexcept override { return msg_.c_str(); }

 private:
  const int connection_;
  const std::string_view context_;

  std::string msg_;
};

class WaiterGotDifferentState : public std::exception {
 public:
  WaiterGotDifferentState(int con, std::string_view expected,
                          std::string_view got)
      : connection_(con), expected_(expected), got_(got) {
    std::stringstream ss;

    ss << "Waiter of connection " << connection_ << " expected: " << expected_
       << ", but got: " << got_;

    msg_ = ss.str();
  }

  int connection() const noexcept { return connection_; }

  std::string_view expected() const noexcept { return expected_; }

  std::string_view got() const noexcept { return got_; }

  const char* what() const noexcept override { return msg_.c_str(); }

 private:
  const int connection_;
  const std::string_view expected_;
  const std::string_view got_;

  std::string msg_;
};

class ConnectionStateError final : public std::exception {
 public:
  ConnectionStateError(int con, std::string_view msg)
      : connection_(con), msg_(msg) {}

  const char* what() const noexcept override { return msg_.c_str(); }

  int connection() const noexcept { return connection_; }

 private:
  const int connection_;

  const std::string msg_;
};

}  // namespace Cnm::Connection::Exceptions

#endif  // HPP_CNM_LIB_CONNECTION_EXCEPTIONS_HPP
