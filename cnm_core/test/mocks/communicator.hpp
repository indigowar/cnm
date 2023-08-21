#ifndef HPP_CNM_CORE_TEST_MOCKS_COMMUNICATOR_HPP
#define HPP_CNM_CORE_TEST_MOCKS_COMMUNICATOR_HPP

#include <gmock/gmock.h>

#include "cnm/machine/communicator.hpp"

class MockCommunicator : public Cnm::Communicator {
  MOCK_METHOD(std::vector<Cnm::HostInfo>, getOfficeEquipments, (bool),
              (override));

  MOCK_METHOD(std::vector<Cnm::HostInfo>, getServers, (bool), (override));

  MOCK_METHOD(std::vector<Cnm::HostInfo>, getPCs, (), (override));

  MOCK_METHOD(void, disconnect, (Cnm::HostInfo), (override));

  MOCK_METHOD(void, makeConnection, (std::string_view), (override));
};

#endif  // HPP_CNM_CORE_TEST_MOCKS_COMMUNICATOR_HPP