#ifndef HPP_CNM_CORE_CONNECTION_CTX_FACTORY_HPP
#define HPP_CNM_CORE_CONNECTION_CTX_FACTORY_HPP

#include <memory>

#include "cnm/connection/connection.hpp"
#include "cnm/connection/internal/client_node.hpp"
#include "cnm/connection/internal/server_node.hpp"

namespace Cnm {

// ContextFactory - builds Client and Server contexts for Connection
class ContextFactory {
 public:
  template <class CtxType>
  static std::unique_ptr<CtxType> createClientContext(
      std::shared_ptr<Connection> connection, Connections::ClientT node) {
    return std::make_unique<CtxType>(connection, node);
  }

  template <class CtxType>
  static std::unique_ptr<CtxType> createServerContext(
      std::shared_ptr<Connection> connection, Connections::ServerT node) {
    return std::make_unique<CtxType>(connection, node);
  }
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_CONNECTION_CTX_FACTORY_HPP
