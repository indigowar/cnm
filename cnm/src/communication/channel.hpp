#ifndef HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP
#define HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP

#include <list>

namespace cnm::communication {

template <class package_t>
class channel final {
 public:
  class unbuffered_error : public std::exception {};

  class closed_channel_error : public std::exception {};

  static channel<package_t> make_with_limit(size_t limit_value);

  static channel<package_t> make_unbuffered();

  bool is_buffered() const noexcept;
  bool is_closed() const noexcept;

  size_t get_limit() const;

  size_t get_amount_of_contained() const noexcept;

  void close();

  channel<package_t>& operator<<(package_t value) {}

  channel<package_t>& operator>>(package_t& value) {}

 private:
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP