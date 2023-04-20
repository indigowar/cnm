#ifndef HPP_CNM_LIB_COMMUNICATION_HPP
#define HPP_CNM_LIB_COMMUNICATION_HPP

#include <memory>
#include <mutex>
#include <shared_mutex>

#include "communication/context.hpp"
#include "communication/message.hpp"

namespace cnm::communication {

/**
 * @brief connection is an interface for all connections in the network
 */
class connection {
 public:
  explicit connection(size_t);

  // get the speed of this network
  [[nodiscard]] size_t net_speed() const noexcept;

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
  virtual void abort(int) noexcept = 0;

  /**
   *
   * @return true - if master can receive multiple messages, otherwise - false.
   */
  [[nodiscard]] virtual bool is_master_buffered() const noexcept = 0;
  /**
   *
   * @return true - if server can receive multiple messages, otherwise - false.
   */
  [[nodiscard]] virtual bool is_server_buffered() const noexcept = 0;

  /**
   *
   * @return the master context of connection(can be received once).
   */
  std::unique_ptr<context> master_ctx() noexcept;

  /**
   *
   * @return the slave(server) context of connection(can be received once).
   */
  std::unique_ptr<context> slave_ctx() noexcept;

 protected:
  std::shared_mutex m_mutex;

 private:
  int m_initiator;  // the one, who creates a connection
  int m_server;     // the one, who serves the connection

  // contexts
  std::unique_ptr<context> m_master_ctx;
  std::unique_ptr<context> m_slave_ctx;

  // the speed of the network,
  // every write/read operation requires (message size / network speed ) time.
  size_t m_net_speed;
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_HPP