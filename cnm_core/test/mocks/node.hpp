#ifndef HPP_CNM_TEST_MOCK_NODE_HPP
#define HPP_CNM_TEST_MOCK_NODE_HPP

#include <gmock/gmock.h>

#include <cnm/topology/base/node.hpp>

class MockNode : public Cnm::Node {
 public:
  MockNode() : Cnm::Node(Cnm::HostInfo{}) {}
};

#endif  // HPP_CNM_TEST_MOCK_NODE_HPP
