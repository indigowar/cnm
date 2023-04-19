#include "communication/context.hpp"

#include "communication/connection.hpp"

using namespace cnm::communication;

context::context(cnm::communication::connection &con, int id)
    : m_connection{con},
      m_id{id},
      m_happened_first_read{},
      m_happened_first_write{} {}

void context::abort() {
  std::unique_lock lock(m_mutex);
  m_connection.abort(m_id);
}

context &context::operator<<(cnm::communication::message &&write_msg) {
  if (m_happened_first_write && !buffered_write_channel_check(m_connection)) {
    throw std::runtime_error(
        "the only available message already has been sent to the participant");
  }
  m_connection.write(m_id, std::move(write_msg));
  m_happened_first_write = true;
  return *this;
}

context &context::operator>>(cnm::communication::message &read_msg) {
  std::unique_lock lock(m_mutex);
  if (m_happened_first_read && !buffered_read_channel_check(m_connection)) {
    throw std::runtime_error(
        "the only available message already has been received");
  }
  read_msg = m_connection.read(m_id);
  m_happened_first_read = true;
  return *this;
}