#ifndef HPP_CNM_CORE_TEST_MOCKS_SLEEP_WRAPPER_HPP
#define HPP_CNM_CORE_TEST_MOCKS_SLEEP_WRAPPER_HPP

#include <gmock/gmock.h>

#include <cnm/utils/sleep_wrapper.hpp>

class MockSleepWrapper : public Utils::SleepWrapper {
  MOCK_METHOD(void, sleepFor, (const std::chrono::milliseconds&), (const));
};

#endif  // HPP_CNM_CORE_TEST_MOCKS_SLEEP_WRAPPER_HPP
