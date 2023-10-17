#include "cnm/connection/connection.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mocks/client_node.hpp"
#include "mocks/intermediate_node.hpp"
#include "mocks/node.hpp"
#include "mocks/server_node.hpp"

using namespace Cnm;

using ::testing::NiceMock;

class ConnectionTest : public ::testing::Test {
 protected:
  void SetUp() override {
    client_node = std::make_shared<NiceMock<MockNode>>();
    intermediate_node = std::make_shared<NiceMock<MockNode>>();
    server_node = std::make_shared<NiceMock<MockNode>>();

    std::vector<std::shared_ptr<Node>> nodes{client_node, intermediate_node,
                                             server_node};

    connection =
        std::make_unique<Connection>(net_speed, nodes.begin(), nodes.end());
  }

  void TearDown() override {
    connection.reset();
    client_node.reset();
    intermediate_node.reset();
    server_node.reset();
  }

  const size_t net_speed = 15;

  std::shared_ptr<NiceMock<MockNode>> client_node;
  std::shared_ptr<NiceMock<MockNode>> intermediate_node;
  std::shared_ptr<NiceMock<MockNode>> server_node;

  std::unique_ptr<Connection> connection;
};

TEST_F(ConnectionTest, CreatingConnectionWithLessThenTwo) {
  std::vector<std::shared_ptr<Cnm::Node>> nodes{
      std::make_shared<NiceMock<MockNode>>()};

  ASSERT_THROW(Connection connection(1500, nodes.begin(), nodes.end()),
               std::runtime_error);
}

TEST_F(ConnectionTest, CreationOfClientContext) {
  auto ctx_result = connection->createClientContext();
  ASSERT_TRUE(ctx_result.isOk());
  ASSERT_FALSE(ctx_result.isErr());

  auto ctx_result_second = connection->createClientContext();

  ASSERT_FALSE(ctx_result_second.isOk());
  ASSERT_TRUE(ctx_result_second.isErr());
}

TEST_F(ConnectionTest, CreationOfServerContext) {
  auto ctx_result = connection->createServerContext();
  ASSERT_TRUE(ctx_result.isOk());
  ASSERT_FALSE(ctx_result.isErr());

  auto ctx_result_second = connection->createServerContext();

  ASSERT_FALSE(ctx_result_second.isOk());
  ASSERT_TRUE(ctx_result_second.isErr());
}

TEST_F(ConnectionTest, CreationOfBothContexts) {
  auto client = connection->createClientContext();
  auto server = connection->createServerContext();

  ASSERT_TRUE(client.isOk());
  ASSERT_FALSE(client.isErr());

  ASSERT_TRUE(server.isOk());
  ASSERT_FALSE(server.isErr());
}

TEST_F(ConnectionTest, GetSpeed) {
  ASSERT_EQ(net_speed, connection->getSpeed());
}