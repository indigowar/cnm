#ifndef HPP_CNM_LIB_COMMUNICATION_CONTEXT_HPP
#define HPP_CNM_LIB_COMMUNICATION_CONTEXT_HPP

#include <mutex>

#include "communication/message.hpp"

namespace cnm::communication {

class connection;

/**
 * @brief the main class of connection's context
 *
 * @details
 * This class hides all internal work with connection classes.
 * It simplifies it to just read and write operations and abort call.
 *
 */
class context {
 public:
  /**
   *
   * @param con - connection
   * @param id  - given to this context id
   */
  context(connection& con, int id);

  virtual ~context() = default;

  /**
   * @brief calling connection::abort()
   *
   * @see connection
   */
  void abort();

  /**
   * @brief write message to the connection
   */
  void write(message&&);

  /**
   * @brief reads a message
   * @return a received message
   */
  message read();

  context(const context&) = delete;
  context& operator=(const context&) = delete;

  context(context&&) = delete;
  context& operator=(context&&) = delete;

 protected:
  /**
   * @return checks is context can write multiple messages to it
   */
  virtual bool buffered_write_channel_check(connection&) = 0;

  /**
   * @return checks is context can read multiple messages to it
   */
  virtual bool buffered_read_channel_check(connection&) = 0;

 private:
  connection& m_connection;
  int m_id;

  std::mutex m_mutex;

  bool m_happened_first_read;
  bool m_happened_first_write;
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_CONTEXT_HPP
