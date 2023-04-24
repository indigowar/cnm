#ifndef HPP_CNM_LIB_COMMUNICATION_MASTER_CTX_HPP
#define HPP_CNM_LIB_COMMUNICATION_MASTER_CTX_HPP

#include "connection.hpp"
#include "context.hpp"

namespace cnm::communication {
class master_ctx : public context {
 public:
  master_ctx(connection& con, int id) : context(con, id) {}

 protected:
  bool buffered_write_channel_check(connection& con) override {
    return con.is_server_buffered();
  }
  bool buffered_read_channel_check(connection& con) override {
    return con.is_master_buffered();
  }
};
}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_MASTER_CTX_HPP
