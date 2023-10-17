#include "cnm/connection/internal/server_node.hpp"

#include <gtest/gtest.h>

#include "mocks/base_connection.hpp"
#include "mocks/connection_node.hpp"
#include "mocks/node.hpp"
#include "mocks/sleep_wrapper.hpp"

using namespace Cnm;
using namespace Cnm::Connections;

using ::testing::NiceMock;
using ::testing::Return;

class ServerNodeTest : public ::testing::Test {
 protected:
  void SetUp() override {
    connection = std::make_shared<NiceMock<MockBaseConnection>>();
    node = std::make_shared<NiceMock<MockNode>>();
    sleep_wrapper = std::make_shared<NiceMock<MockSleepWrapper>>();

    server_node =
        std::make_unique<ServerNode>(*connection, node, sleep_wrapper);

    connection_node = std::make_shared<NiceMock<MockConnectionNode>>(
        *connection, node, sleep_wrapper);
  }

  void TearDown() override {
    server_node.reset();
    connection_node.reset();
    sleep_wrapper.reset();
    node.reset();
    connection.reset();
  }

  std::shared_ptr<MockBaseConnection> connection;
  std::shared_ptr<MockNode> node;
  std::shared_ptr<MockSleepWrapper> sleep_wrapper;

  std::shared_ptr<MockConnectionNode> connection_node;

  std::unique_ptr<ServerNode> server_node;
};

TEST_F(ServerNodeTest, NextNode) {
  EXPECT_THROW([this] { server_node->setNextNode(connection_node); }(),
               std::runtime_error);

  EXPECT_EQ(server_node->getNextNode(), nullptr);
}

TEST_F(ServerNodeTest, PreviousNode) {
  server_node->setPreviousNode(connection_node);

  EXPECT_EQ(server_node->getPreviousNode(), connection_node);
}

TEST_F(ServerNodeTest, sendForwardWhenConnectionIsRequesting) {
  const size_t amount = 4;
  EXPECT_CALL(*connection, isRequesting())
      .Times(amount)
      .WillRepeatedly(Return(true));

  for (size_t i{}; i != amount; i++) {
    server_node->sendForward(Message{});
  }

  EXPECT_EQ(server_node->getBuffer().size(), amount);
}

TEST_F(ServerNodeTest, sendForwardWhenConnectionIsNotRequesting) {
  EXPECT_CALL(*connection, isRequesting()).WillOnce(Return(false));

  server_node->sendForward(Message{});

  EXPECT_EQ(server_node->getBuffer().size(), 0);
}

TEST_F(ServerNodeTest, sendBackwardWhenConnectionIsServingAndHasPrevious) {
  EXPECT_CALL(*connection, isServing()).WillOnce(Return(true));
  EXPECT_CALL(*connection_node, sendBackward).Times(1);
  EXPECT_CALL(*sleep_wrapper, sleepFor).Times(1);

  server_node->setPreviousNode(connection_node);

  server_node->sendBackward(Message{});
}

TEST_F(ServerNodeTest, sendBackwardWhenConnectionIsServingButNoPreviousNode) {
  EXPECT_CALL(*connection, isServing()).WillOnce(Return(true));
  EXPECT_CALL(*connection, abort()).Times(2);  // on the error, and on the DTOR.

  // sending without having a previous node.
  server_node->sendBackward(Message{});
}
