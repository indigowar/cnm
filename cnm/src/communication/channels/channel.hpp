#ifndef HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP
#define HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP

#include <future>

namespace cnm::communication {

/**
 * @brief channel class is the tool to interprocess communication.
 * @tparam T - the type of the package
 */
template <class T>
class channel {
  virtual ~channel() = default;

  /**
   * @brief write the value to the channel
   */
  virtual void write(T) = 0;

  /**
   * @brief read the value from the channel
   */
  virtual std::future<T> read() = 0;

  /**
   *
   * @return true if the channel is closed now
   */
  [[nodiscard]] virtual bool is_closed() const noexcept = 0;

  /**
   * @brief close the channel(can't read or write)
   */
  virtual void close() = 0;

  /**
   *
   * @return the max number of packages it can send
   */
  virtual size_t limit() const noexcept = 0;
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP