#ifndef HPP_CNM_LIB_CONNECTION_BUILDER_HPP
#define HPP_CNM_LIB_CONNECTION_BUILDER_HPP

#include <memory>

#include "connection/connection.hpp"

namespace Cnm::Connection {

class Builder {
 public:
  explicit Builder(size_t net_speed);

  std::unique_ptr<Connection> makeOneToOne();
  std::unique_ptr<Connection> makeOneToMany();
  std::unique_ptr<Connection> makeManyToOne();
  std::unique_ptr<Connection> makeManyToMany();

 private:
  size_t net_speed_;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_LIB_CONNECTION_BUILDER_HPP
