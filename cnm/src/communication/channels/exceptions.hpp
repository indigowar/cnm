#ifndef HPP_CNM_LIB_COMMUNICATION_CHANNELS_HPP
#define HPP_CNM_LIB_COMMUNICATION_CHANNELS_HPP

#include <exception>
#include <sstream>
#include <string>

namespace cnm::communication::exceptions {

class channel_unbuffered_error : public std::exception {};


class channel_closed_error : public std::exception {};

class channel_overflowed_error : public std::exception {
 public:
  explicit channel_overflowed_error(size_t limit) {
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
}  // namespace cnm::communication::exceptions

#endif  // HPP_CNM_LIB_COMMUNICATION_CHANNELS_HPP
