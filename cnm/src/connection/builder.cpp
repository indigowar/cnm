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
  auto result =
      std::make_unique<Connection>(makeChannelForOne(), makeChannelForOne());
  result->setSpeed(net_speed_);
  return result;
}

std::unique_ptr<Connection> Builder::makeOneToMany() {
  auto result =
      std::make_unique<Connection>(makeChannelForOne(), makeChannelForMany());
  result->setSpeed(net_speed_);
  return result;
}

std::unique_ptr<Connection> Builder::makeManyToOne() {
  auto result =
      std::make_unique<Connection>(makeChannelForMany(), makeChannelForOne());
  result->setSpeed(net_speed_);
  return result;
}

std::unique_ptr<Connection> Builder::makeManyToMany() {
  auto result =
      std::make_unique<Connection>(makeChannelForMany(), makeChannelForMany());
  result->setSpeed(net_speed_);
  return result;
}

}  // namespace Cnm::Connection
