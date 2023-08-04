#ifndef HPP_CNM_CORE_CONNECTION_CLIENT_CTX_HPP
#define HPP_CNM_CORE_CONNECTION_CLIENT_CTX_HPP

#include <memory>

#include "cnm/connection/internal/client_node.hpp"
#include "cnm/connection/internal/connection.hpp"

namespace Cnm {

class ClientContext final {
 public:
  explicit ClientContext(Connections::Connection*,
                         std::shared_ptr<Connections::ClientNode>);

 private:
  Connections::Connection* connection;
  std::shared_ptr<Connections::ClientNode> node;
};

using ClientCtx = std::unique_ptr<ClientContext>;

}  // namespace Cnm

#endif  // HPP_CNM_CORE_CONNECTION_CLIENT_CTX_HPP
