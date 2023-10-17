#ifndef HPP_CNM_CORE_TEST_MOCKS_CLIENT_NODE_HPP
#define HPP_CNM_CORE_TEST_MOCKS_CLIENT_NODE_HPP

#include <gmock/gmock.h>

#include "mocks/connection_node.hpp"

class MockClientNode : public MockConnectionNode {
 public:
  MOCK_METHOD(const std::vector<Cnm::Message>&, getBuffer, (),
              (const, noexcept));
};

#endif  // HPP_CNM_CORE_TEST_MOCKS_CLIENT_NODE_HPP
