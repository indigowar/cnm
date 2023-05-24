#ifndef HPP_CNM_LIB_COMMUNICATION_CHANNELS_HPP
#define HPP_CNM_LIB_COMMUNICATION_CHANNELS_HPP

#include <exception>
#include <sstream>
#include <string>

namespace Cnm::Communication::Exceptions {

class ChannelUnbuffered : public std::exception {};

class ChannelIsClosed : public std::exception {};

class ChannelIsOverflowed : public std::exception {
 public:
  explicit ChannelIsOverflowed(size_t limit) {
    std::stringstream ss;
    ss << "Channel is overflowed. It's max value is: " << limit << ".";
    msg = ss.str();
  }

  [[nodiscard]] const char* what() const noexcept override {
    return msg.c_str();
  }

 private:
  std::string msg;
};
}  // namespace Cnm::Communication::Exceptions

#endif  // HPP_CNM_LIB_COMMUNICATION_CHANNELS_HPP
