#ifndef HPP_CNM_TEST_MOCK_NODE_HPP
#define HPP_CNM_TEST_MOCK_NODE_HPP

#include <gmock/gmock.h>

#include <cnm/connection/internal/connection.hpp>

class MockNode : public Cnm::Connections::_Node {
  MOCK_METHOD(Cnm::HostInfo, getHostInfo, (), (const, noexcept, override));
};

#endif  // HPP_CNM_TEST_MOCK_NODE_HPP
