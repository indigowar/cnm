#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cnm/connection/internal/intermediate_node.hpp>

#include "mocks/connection.hpp"
#include "mocks/node.hpp"
#include "mocks/sleep_wrapper.hpp"

using namespace Cnm;
using namespace Cnm::Connections;

class IntermediateNodeTest : public ::testing::Test {
 protected:
  void SetUp() override {
    sleep_wrapper = std::make_shared<testing::NiceMock<MockSleepWrapper>>();
    connection = std::make_shared<testing::NiceMock<MockConnection>>();
    node = std::make_shared<testing::NiceMock<MockNode>>();

    intermediate_node =
        std::make_unique<IntermediateNode>(*connection, node, *sleep_wrapper);
  }

  void TearDown() override {
    intermediate_node.reset();
    node.reset();
    connection.reset();
    sleep_wrapper.reset();
  }

  std::unique_ptr<IntermediateNode> intermediate_node;
  std::shared_ptr<MockNode> node;
  std::shared_ptr<MockConnection> connection;

  std::shared_ptr<MockSleepWrapper> sleep_wrapper;
};

TEST_F(IntermediateNodeTest, SetNextNode) {
  auto next_node =
      std::make_shared<IntermediateNode>(*connection, node, MockSleepWrapper());
  intermediate_node->setNextNode(next_node);
  EXPECT_EQ(intermediate_node->getNextNode(), next_node);
}

TEST_F(IntermediateNodeTest, SetPreviousNode) {
  auto previous =
      std::make_shared<IntermediateNode>(*connection, node, MockSleepWrapper());
  intermediate_node->setPreviousNode(previous);
  EXPECT_EQ(intermediate_node->getPreviousNode(), previous);
}

TEST_F(IntermediateNodeTest, getNextNode) {
  auto next =
      std::make_shared<IntermediateNode>(*connection, node, MockSleepWrapper());

  intermediate_node->setNextNode(next);

  EXPECT_EQ(intermediate_node->getNextNode(), next);
}
