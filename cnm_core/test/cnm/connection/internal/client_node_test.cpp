#include "cnm/connection/internal/client_node.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

#include "mocks/base_connection.hpp"
#include "mocks/connection_node.hpp"
#include "mocks/node.hpp"
#include "mocks/sleep_wrapper.hpp"

using namespace Cnm;
using namespace Cnm::Connections;

using ::testing::NiceMock;
using ::testing::Return;

class ClientNodeTest : public ::testing::Test {
 protected:
  void SetUp() override {
    connection = std::make_shared<NiceMock<MockBaseConnection>>();
    node = std::make_shared<NiceMock<MockNode>>();
    sleep_wrapper = std::make_shared<NiceMock<MockSleepWrapper>>();
    connection_node = std::make_shared<NiceMock<MockConnectionNode>>(
        *connection, node, sleep_wrapper);

    client_node =
        std::make_unique<ClientNode>(*connection, node, sleep_wrapper);
  }

  void TearDown() override {
    client_node.reset();
    connection_node.reset();
    sleep_wrapper.reset();
    connection.reset();
  }

  std::shared_ptr<NiceMock<MockBaseConnection>> connection;
  std::shared_ptr<NiceMock<MockNode>> node;
  std::shared_ptr<NiceMock<MockConnectionNode>> connection_node;
  std::shared_ptr<NiceMock<MockSleepWrapper>> sleep_wrapper;

  std::unique_ptr<ClientNode> client_node;
};

TEST_F(ClientNodeTest, setNextNode) {
  client_node->setNextNode(connection_node);
  EXPECT_EQ(client_node->getNextNode(), connection_node);
}

TEST_F(ClientNodeTest, UsageOfPreviousNode) {
  EXPECT_EQ(client_node->getPreviousNode(), nullptr);

  EXPECT_THROW([this] { client_node->setPreviousNode(connection_node); }(),
               std::runtime_error);
}

TEST_F(ClientNodeTest, sendForwardWhenConnectionIsRequesting) {
  EXPECT_CALL(*connection, isRequesting()).WillOnce(Return(true));

  client_node->setNextNode(connection_node);
  EXPECT_CALL(*connection_node, sendForward).Times(1);

  client_node->sendForward(Message{});
}

TEST_F(ClientNodeTest, sendForwardWhenConnectionIsNotRequsting) {
  EXPECT_CALL(*connection, isRequesting()).WillOnce(Return(false));

  EXPECT_CALL(*connection, abort()).Times(1);

  client_node->sendForward(Message{});
}

TEST_F(ClientNodeTest, sendBackwardWhenConnectionIsServing) {
  const size_t amount = 4;

  EXPECT_CALL(*connection, isServing())
      .Times(amount)
      .WillRepeatedly(Return(true));

  for (size_t i{}; i != amount; i++) {
    client_node->sendBackward(Message{});
  }

  EXPECT_EQ(client_node->getBuffer().size(), amount);
}

TEST_F(ClientNodeTest, sendBackwardWhenConnectionIsNotServing) {
  EXPECT_CALL(*connection, isServing()).Times(1).WillOnce(Return(false));

  client_node->sendBackward(Message{});

  EXPECT_NE(client_node->getBuffer().size(), 1);
}
