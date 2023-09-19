#ifndef HPP_CNM_TEST_MOCK_MACHINE_HPP
#define HPP_CNM_TEST_MOCK_MACHINE_HPP

#include <gmock/gmock.h>

#include <cnm/machine/machine.hpp>

class MockMachine : public Cnm::Machine {
 public:
  MOCK_METHOD(void, start, (), (override));

  MOCK_METHOD(void, stop, (), (override));

  MOCK_METHOD(void, invoke, (), (override));

  MOCK_METHOD(void, freeze, (), (override));

  MOCK_METHOD(Object::Status, getStatus, (), (const, noexcept, override));

  MOCK_METHOD(std::string_view, getType, (), (const, noexcept));

  MOCK_METHOD(size_t, getServingLimit, (), (const, noexcept));

  MOCK_METHOD(Cnm::HostInfo, getHostInfo, (), (const, noexcept));

  MOCK_METHOD(void, setHostInfo, (Cnm::HostInfo), ());

  MOCK_METHOD(void, setCommunicator, (std::unique_ptr<Cnm::Communicator>&&),
              ());

  MOCK_METHOD(size_t, getCurrentServingAmount, (), (const, noexcept, override));

  MOCK_METHOD(std::unique_lock<std::mutex>, makeLock, (), (const, noexcept));

  MOCK_METHOD(void, serve, (Cnm::ServerCtx&&), (override));
};

#endif  // HPP_CNM_TEST_MOCK_MACHINE_HPP
