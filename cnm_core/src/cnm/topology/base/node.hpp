#ifndef HPP_CNM_CORE_TOPOLOGY_BASE_NODE_HPP
#define HPP_CNM_CORE_TOPOLOGY_BASE_NODE_HPP

#include <memory>
#include <vector>

#include "cnm/core/object.hpp"
#include "cnm/machine/host_info.hpp"

namespace Cnm {

class ServerContext;

class Node;

namespace Connections {
class ConnectionNode;
}  // namespace Connections

struct ConnectionInfo final {
  HostInfo client;
  HostInfo server;
};

class Node : public Object {
 public:
  [[nodiscard]] virtual HostInfo getHostInfo() const noexcept = 0;
  virtual void setHostInfo(HostInfo) = 0;

  [[nodiscard]] virtual bool isServing() const noexcept = 0;

  [[nodiscard]] virtual bool isBusy() const noexcept = 0;

  [[nodiscard]] virtual std::vector<std::shared_ptr<Node>> getConnectedNodes()
      const noexcept = 0;

  virtual void serve(std::unique_ptr<ServerContext>&&) = 0;

  [[nodiscard]] virtual std::string_view getType() const noexcept = 0;

  virtual void attachConnectionNode(Connections::ConnectionNode*) = 0;
  virtual void detachConnectionNode(Connections::ConnectionNode*) = 0;

  virtual std::vector<ConnectionInfo> getConnections() const noexcept = 0;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_BASE_NODE_HPP
