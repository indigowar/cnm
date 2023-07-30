#ifndef HPP_CNM_TEST_MOCKS_BASE_CONNECTION_HPP
#define HPP_CNM_TEST_MOCKS_BASE_CONNECTION_HPP

#include <gmock/gmock.h>

#include <cnm/connection/internal/connection.hpp>

class MockBaseConnection : public Cnm::Connections::Connection {
 public:
  MOCK_METHOD(void, abort, (), (override));
  MOCK_METHOD(std::shared_ptr<Cnm::Connections::ConnectionNode>, addNode,
              (std::shared_ptr<Cnm::Connections::_Node>), (override));
  MOCK_METHOD(bool, isRequesting, (), (const, noexcept, override));
  MOCK_METHOD(bool, isServing, (), (const, noexcept, override));
  MOCK_METHOD(std::unique_lock<std::mutex>, lock, (), (override));
  MOCK_METHOD(size_t, getSpeed, (), (const, noexcept, override));
};

#endif  // HPP_CNM_TEST_MOCKS_BASE_CONNECTION_HPP
