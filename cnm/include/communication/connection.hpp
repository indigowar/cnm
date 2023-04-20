#ifndef HPP_CNM_LIB_COMMUNICATION_HPP
#define HPP_CNM_LIB_COMMUNICATION_HPP

#include <future>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include "communication/context.hpp"
#include "communication/message.hpp"

namespace cnm::communication {

/**
 * @class connection
 *
 * @brief base class for different types of connection.
 *
 * @details This class is base class for connections, i.e. One-to-One,
 * One-to-Many and so on. This class contains base logic of all connections,
 * f.e. creation of contexts, but leaves open an interfaces for connection.
 *
 * @author Max Kamerov
 */
class connection {
 public:
  /**
   * @brief connection constructor
   *
   * @arg size_t speed - the speed of the network.
   */
  explicit connection(size_t);

  /**
   * @return gets the speed of the connection that was given in constructor
   */
  [[nodiscard]] size_t net_speed() const noexcept;

  /**
   * @brief write a message to connection, so the participant can retrieve it.
   *
   * @arg1 int id - the unique id of context.
   * @arg2 message& msg - the object to send using connection.
   */
  virtual void write(int, message&&) = 0;

  /**
   * @brief retrieve message in sync way.
   *
   * @details
   * This method retrieves the message from participant.
   * If there is no message then the thread will be immediatly locked.
   * If there is message it will retrieve as a rvalue
   *
   * @arg int id - the id of context.
   *
   * @return received message object.
   */
  virtual message&& read(int) = 0;

  /**
   * @brief abort the connection and stop it form working.
   *
   * @arg int id - the context id.
   */
  virtual void abort(int) noexcept = 0;

  /**
   * @brief return true if master can retrieve multiple responses.
   */
  [[nodiscard]] virtual bool is_master_buffered() const noexcept = 0;

  /**
   * @brief return true if server can retrieve multiple responses.
   */
  [[nodiscard]] virtual bool is_server_buffered() const noexcept = 0;

  /**
   * @brief return true if master has responses to receive.
   */
  [[nodiscard]] virtual bool is_master_chan_empty() const noexcept = 0;

  /**
   * @brief return true if server has requests to receive.
   */
  [[nodiscard]] virtual bool is_server_chan_empty() const noexcept = 0;

  /**
   * @brief the async read
   *
   * @details
   * This method is async implementation of read method.
   * It reads the channel for participant(defined by context id),
   * but if there's no available responses you will retrieve an empty
   * std::future.
   * @return
   */
  [[nodiscard]] virtual std::future<message> read_async(int) noexcept = 0;

  /**
   * @brief one-time receiver of master context.
   *
   * @return  master context
   */
  std::unique_ptr<context> master_ctx() noexcept;
  /**
   * @brief one-time receiver of slave(server) context.
   *
   * @return slave context
   */
  std::unique_ptr<context> slave_ctx() noexcept;

 protected:
  /**
   * @brief this mutex is here to be able to synchronize the access to fields.
   *
   * @warning probably will be deleted, because it's a bad pattern to have it
   * like that.
   */
  std::shared_mutex m_mutex;

 private:
  // the id of master(initiator, the one who starts the connection).
  int m_initiator;
  // the id of slave(server, the one who receives requests and do not start the
  // connection).
  int m_server;

  // master connection
  std::unique_ptr<context> m_master_ctx;
  // slave context
  std::unique_ptr<context> m_slave_ctx;

  // the speed of network(it should be used in write/read operations).
  size_t m_net_speed;
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_HPP