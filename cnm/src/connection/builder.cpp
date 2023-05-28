#include "builder.hpp"

#include <memory>

#include "communication/channels/channel.hpp"

namespace Cnm::Connection {

constexpr size_t LIMIT_FOR_MANY{100};

using namespace Cnm::Communication;

std::shared_ptr<Channel<Message>> makeChannelForOne() {
  return std::make_shared<Channel<Message>>(channel_storage<Message>(1));
}

std::shared_ptr<Channel<Message>> makeChannelForMany() {
  return std::make_shared<Channel<Message>>(
      channel_storage<Message>(LIMIT_FOR_MANY));
}

Builder::Builder(size_t net_speed) : net_speed_{net_speed} {}

std::unique_ptr<Connection> Builder::makeOneToOne() {
  return std::make_unique<Connection>(net_speed_, makeChannelForOne(),
                                      makeChannelForOne());
}

std::unique_ptr<Connection> Builder::makeOneToMany() {
  return std::make_unique<Connection>(net_speed_, makeChannelForOne(),
                                      makeChannelForMany());
}

std::unique_ptr<Connection> Builder::makeManyToOne() {
  return std::make_unique<Connection>(net_speed_, makeChannelForMany(),
                                      makeChannelForOne());
}

std::unique_ptr<Connection> Builder::makeManyToMany() {
  return std::make_unique<Connection>(net_speed_, makeChannelForMany(),
                                      makeChannelForMany());
}

}  // namespace Cnm::Connection
