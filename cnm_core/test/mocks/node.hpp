#ifndef HPP_CNM_TEST_MOCK_NODE_HPP
#define HPP_CNM_TEST_MOCK_NODE_HPP

#include <gmock/gmock.h>

#include "cnm/topology/base/node.hpp"
#include "gmock/gmock.h"

class MockNode : public Cnm::Node {
 public:
  MOCK_METHOD(Cnm::HostInfo, getHostInfo, (), (const, noexcept, override));
  MOCK_METHOD(void, setHostInfo, (Cnm::HostInfo), (override));

  MOCK_METHOD(bool, isServing, (), (const, noexcept, override));
  MOCK_METHOD(std::vector<std::shared_ptr<Cnm::Node>>, getConnectedNodes, (),
              (const, noexcept, override));
  MOCK_METHOD(void, serve, (std::unique_ptr<Cnm::ServerContext>&&), (override));

  MOCK_METHOD(void, start, (), (override));
  MOCK_METHOD(void, stop, (), (override));
  MOCK_METHOD(void, invoke, (), (override));
  MOCK_METHOD(void, freeze, (), (override));

  MOCK_METHOD(std::string_view, getType, (), (const, noexcept, override));

  MOCK_METHOD(void, attachConnectionNode, (Cnm::Connections::ConnectionNode*),
              (override));

  MOCK_METHOD(void, detachConnectionNode, (Cnm::Connections::ConnectionNode*),
              (override));
};

#endif  // HPP_CNM_TEST_MOCK_NODE_HPP
