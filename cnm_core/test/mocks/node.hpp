#ifndef HPP_CNM_TEST_MOCK_NODE_HPP
#define HPP_CNM_TEST_MOCK_NODE_HPP

#include <gmock/gmock.h>

#include <cnm/topology/base/node.hpp>

class MockNode : public Cnm::Node {
 public:
  MockNode() : Cnm::Node(Cnm::HostInfo("", "")) {}

  MOCK_METHOD(void, start, (), (override));
  MOCK_METHOD(void, stop, (), (override));
  MOCK_METHOD(void, invoke, (), (override));
  MOCK_METHOD(void, freeze, (), (override));
};

#endif  // HPP_CNM_TEST_MOCK_NODE_HPP
