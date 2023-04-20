#ifndef HPP_CNM_LIB_COMMUNICATION_CONTEXT_HPP
#define HPP_CNM_LIB_COMMUNICATION_CONTEXT_HPP

#include <mutex>

#include "communication/message.hpp"

namespace cnm::communication {

class connection;

class context {
 public:
  context(connection& con, int id);

  virtual ~context() = default;

  void abort();

  void write(message&&);

  message read();

  context(const context&) = delete;
  context& operator=(const context&) = delete;

  context(context&&) = delete;
  context& operator=(context&&) = delete;

 protected:
  virtual bool buffered_write_channel_check(connection&) = 0;
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
