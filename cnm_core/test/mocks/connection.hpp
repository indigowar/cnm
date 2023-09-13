#ifndef HPP_CNM_CORE_TEST_MOCKS_CONNECTION_HPP
#define HPP_CNM_CORE_TEST_MOCKS_CONNECTION_HPP

#include <gmock/gmock.h>

#include <cnm/connection/internal/connection.hpp>

class MockConnection : public Cnm::Connections::Connection {
 public:
  MOCK_METHOD(void, abort, (), (override));
  MOCK_METHOD(bool, isRequesting, (), (const, noexcept, override));
  MOCK_METHOD(bool, isServing, (), (const, noexcept, override));
  MOCK_METHOD(bool, isAborted, (), (const, noexcept, override));
  MOCK_METHOD(std::unique_lock<std::mutex>, makeLock, (),
              (const, noexcept, override));
  MOCK_METHOD(size_t, getSpeed, (), (const, noexcept, override));
  MOCK_METHOD(void, stopRequesting, (), (override));
  MOCK_METHOD(void, stopServing, (), (override));
  MOCK_METHOD(Cnm::HostInfo, getClientHostInfo, (),
              (const, noexcept, override));
  MOCK_METHOD(Cnm::HostInfo, getServerHostInfo, (),
              (const, noexcept, override));
};

#endif  // HPP_CNM_CORE_TEST_MOCKS_CONNECTION_HPP
