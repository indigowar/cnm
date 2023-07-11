#ifndef HPP_CNM_LIB_CNM_COMMUNICATION_MESSAGE_HPP
#define HPP_CNM_LIB_CNM_COMMUNICATION_MESSAGE_HPP

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace Cnm::Communication {

enum Code : uint16_t {
  None,

  OK = 200,
  Accepted = 201,

  BadRequest = 400,
  NotFound = 404,

  InternalError = 500
};

class Message final {
 public:
  Message() : from_{}, to_{}, value_{}, code_{Code::None} {}

  Message(std::string_view from, std::string_view to, std::string msg,
          Code code = Code::None)
      : from_{from}, to_{to}, value_{msg}, code_{code} {}

  Message(std::string_view from, std::string_view to,
          std::vector<std::byte> data, Code code = Code::None)
      : from_{from}, to_{to}, value_{std::move(data)}, code_{code} {}

  std::string_view from() const noexcept { return from_; }

  std::string_view to() const noexcept { return to_; }

  bool containsBinary() const noexcept {
    return !std::holds_alternative<std::vector<std::byte>>(value_);
  }

  bool containsText() const noexcept {
    return !std::holds_alternative<std::string>(value_);
  }

  Code code() const noexcept { return code_; }

  const std::string getTextData() const {
    return std::get<std::string>(value_);
  }

  const std::vector<std::byte>& getRawData() const {
    return std::get<std::vector<std::byte>>(value_);
  }

  Message(const Message& msg)
      : from_{msg.from_}, to_{msg.to_}, value_{msg.value_} {}

  const Message& operator=(const Message& msg) {
    this->from_ = msg.from_;
    this->to_ = msg.to_;
    this->value_ = msg.value_;
    this->code_ = msg.code_;
    return *this;
  }

  Message(Message&& msg)
      : from_{std::move(msg.from_)},
        to_{std::move(msg.to_)},
        value_{std::move(msg.value_)} {}

  const Message& operator=(Message&& msg) {
    this->from_ = std::move(msg.from_);
    this->to_ = std::move(msg.to_);
    this->value_ = std::move(msg.value_);
    return *this;
  }

 private:
  std::string_view from_;
  std::string_view to_;
  Code code_;

  std::variant<std::vector<std::byte>, std::string> value_;
};

}  // namespace Cnm::Communication

#endif  // HPP_CNM_LIB_CNM_COMMUNICATION_MESSAGE_HPP
