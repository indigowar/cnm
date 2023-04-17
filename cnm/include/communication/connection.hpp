#ifndef HPP_CNM_LIB_COMMUNICATION_HPP
#define HPP_CNM_LIB_COMMUNICATION_HPP

#include <mutex>
#include <shared_mutex>

#include "communication/message.hpp"

namespace cnm::communication {

/**
 * @brief connection is an interface for all connections in the network
 */
class connection {
 public:
  explicit connection(size_t net_speed) : m_net_speed(net_speed) {}

  // get the speed of this network
  size_t net_speed() const noexcept { return m_net_speed; }

  /**
   * @brief write to the participant
   *
   * @arg int id - the id of the caller
   * @arg message&& msg - the message to write
   */
  virtual void write(int, message&&) = 0;

  /**
   * @brief read from participant
   */
  virtual message&& read(int) = 0;

  /**
   * @brief abort the connection
   */
  virtual void abort() noexcept = 0;

  /**
   * @brief get amount of requests you can
   */
  virtual size_t establisher_buff_size() const noexcept = 0;
  virtual size_t server_buff_size() const noexcept = 0;

 protected:
  std::shared_mutex m_mutex;

 private:
  int m_establisher;  // the one, who creates a connection
  int m_server;       // the one, who serves the connection

  // the speed of the network,
  // every write/read operation requires (message size / network speed ) time.
  size_t m_net_speed;
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_HPP