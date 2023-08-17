#include "cnm/machine/host_info.hpp"

#include <gtest/gtest.h>

#include <sstream>

using namespace Cnm;

TEST(HostInfoTest, generate) {
  const std::string_view name = "test";
  const uint8_t first = 125, second = 134, third = 55, fourth = 11;

  std::ostringstream ss;
  ss << first << "." << second << "." << third << "." << fourth;
  std::string address = ss.str();

  auto info = HostInfo::generate(name, first, second, third, fourth);

  EXPECT_EQ(info.getName(), name);
  EXPECT_EQ(info.getAddress(), address);
}
