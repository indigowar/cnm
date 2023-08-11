#ifndef HPP_CNM_CORE_TEST_MOCKS_CONNECTION_NODE_HPP
#define HPP_CNM_CORE_TEST_MOCKS_CONNECTION_NODE_HPP

#include <gmock/gmock.h>

#include <cnm/connection/internal/connection_node.hpp>

class MockConnectionNode : public Cnm::Connections::ConnectionNode {
 public:
  MockConnectionNode(Cnm::Connections::Connection& connection,
                     std::shared_ptr<Cnm::Node> node,
                     const std::shared_ptr<Utils::SleepWrapper>& sw)
      : ConnectionNode(connection, std::move(node), sw) {}

  MOCK_METHOD(void, setNextNode, (std::shared_ptr<ConnectionNode>), (override));
  MOCK_METHOD(std::shared_ptr<ConnectionNode>, getNextNode, (),
              (const, noexcept, override));
  MOCK_METHOD(void, setPreviousNode, (std::shared_ptr<ConnectionNode>),
              (override));
  MOCK_METHOD(std::shared_ptr<ConnectionNode>, getPreviousNode, (),
              (const, noexcept, override));
  MOCK_METHOD(void, sendForward, (Cnm::Message&&), (override));
  MOCK_METHOD(void, sendBackward, (Cnm::Message&&), (override));
  MOCK_METHOD(void, abort, (), (override));
};

#endif  // HPP_CNM_CORE_TEST_MOCKS_CONNECTION_NODE_HPP
