#ifndef HPP_CNM_CORE_CONNECTION_SERVER_CTX_HPP
#define HPP_CNM_CORE_CONNECTION_SERVER_CTX_HPP

#include <memory>

#include "cnm/connection/internal/connection.hpp"
#include "cnm/connection/internal/server_node.hpp"

namespace Cnm {

class ServerContext final {
 public:
  explicit ServerContext(Connections::Connection*,
                         std::shared_ptr<Connections::ServerNode>);

 private:
  Connections::Connection* connection;
};

using ServerCtx = std::unique_ptr<ServerContext>;

}  // namespace Cnm

#endif  // HPP_CNM_CORE_CONNECTION_SERVER_CTX_HPP
