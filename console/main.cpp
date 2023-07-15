
// communication
#include <communication/channels/channel.hpp>
#include <communication/message.hpp>
// ring topology
#include <topology/ring/node.hpp>
#include <topology/ring/ring.hpp>
// machines
#include <machines/machine.hpp>
#include <machines/server.hpp>

//
#include <iostream>

using namespace Cnm;
using namespace Cnm::Machines;
using namespace Cnm::Topology;
using namespace Cnm::Communication;
using namespace Cnm::Connection;

int main(int argc, char** argv) {
  Ring::Topology ring_topology;

  auto logic = [](const HostInfo& info, std::unique_ptr<ServerCtx>&& ctx) {

  };

  HostInfo hi{"127.0.0.5", "server_x"};

  auto m = std::make_unique<Server>(hi, 5, logic);

  return 0;
}
