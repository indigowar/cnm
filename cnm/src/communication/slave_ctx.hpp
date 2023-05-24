#ifndef HPP_CNM_LIB_COMMUNICATION_SLAVE_CTX_HPP
#define HPP_CNM_LIB_COMMUNICATION_SLAVE_CTX_HPP

#include "connection.hpp"
#include "context.hpp"

namespace Cnm::Communication {

class slave_ctx : public context {
 public:
  slave_ctx(connection& con, int id) : context(con, id) {}

 protected:
  bool buffered_write_channel_check(connection& con) override {
    return con.is_master_buffered();
  }
  bool buffered_read_channel_check(connection& con) override {
    return con.is_server_buffered();
  }
};

}  // namespace Cnm::Communication

#endif // HPP_CNM_LIB_COMMUNICATION_SLAVE_CTX_HPP
