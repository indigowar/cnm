#ifndef HPP_CNM_CORE_CORE_MESSAGE_HPP
#define HPP_CNM_CORE_CORE_MESSAGE_HPP

#include <variant>
#include <vector>

namespace Cnm {

// Message that is sending from clients to servers and vice versa.
// This class contains info for one specific message.
using Message = std::string;

// MessageBatch - a class that might contain one or many messages(Message).
class MessageBatch final {
 public:
  MessageBatch() = default;

  explicit MessageBatch(Message&& msg) : data{std::move(msg)} {}

  explicit MessageBatch(std::vector<Message>&& msgs) : data{std::move(msgs)} {}

  MessageBatch(const MessageBatch& other) = default;

  MessageBatch& operator=(const MessageBatch& other) {
    data = other.data;
    return *this;
  }

  MessageBatch(MessageBatch&& other) noexcept : data{std::move(other.data)} {}

  MessageBatch& operator=(MessageBatch&& other) noexcept {
    data = std::move(other.data);
    return *this;
  }

  [[nodiscard]] std::vector<Message> getMessageList() const noexcept {
    return std::get<std::vector<Message>>(data);
  }

  [[nodiscard]] Message getMessage() const noexcept {
    return std::get<Message>(data);
  }

  [[nodiscard]] bool containsOneMessage() const noexcept {
    return !containsMessageList();
  }

  [[nodiscard]] bool containsMessageList() const noexcept {
    return std::holds_alternative<std::vector<Message>>(data);
  }

 private:
  std::variant<Message, std::vector<Message>> data;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_CORE_MESSAGE_HPP
