#ifndef HPP_CNM_CORE_TOPOLOGY_BASE_NODE_HPP
#define HPP_CNM_CORE_TOPOLOGY_BASE_NODE_HPP

// #include <memory>
// #include <vector>

// #include "cnm/connection/server_ctx.hpp"
// #include "cnm/core/object.hpp"
// #include "cnm/machine/communicator.hpp"
// #include "cnm/machine/host_info.hpp"
// #include "cnm/machine/machine.hpp"

namespace Cnm {

class Node {};

// Node is the base class for the nodes in the network.
// class Node : public Object {
//  public:
//   Node(HostInfo host_info, std::unique_ptr<Machine>&& m,
//        const std::shared_ptr<Communicator>& communicator);
//
//   ~Node() override;
//
//   void start() override;
//
//   void stop() override;
//
//   void invoke() override;
//
//   void freeze() override;
//
//   [[nodiscard]] HostInfo getHostInfo() const noexcept;
//
//   void setHostInfo(HostInfo host_info);
//
//   [[nodiscard]] virtual std::vector<std::shared_ptr<Node>>
//   getConnectedNodes()
//       const noexcept;
//
//   [[nodiscard]] bool isServing() const noexcept;
//
//   void serve(ServerCtx&& ctx);
//
//  private:
//   std::unique_ptr<Machine> machine;
// };

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_BASE_NODE_HPP
