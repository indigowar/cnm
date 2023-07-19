#include <gtest/gtest.h>

#include <cnm/utils/result.hpp>

auto result_ok = Result<int, std::string>::Ok(42);
auto result_err = Result<int, std::string>::Err("error");

TEST(ResultTest, IsOk) {
  EXPECT_TRUE(result_ok.is_ok());
  EXPECT_FALSE(result_ok.is_err());
  EXPECT_FALSE(result_err.is_ok());
  EXPECT_TRUE(result_err.is_err());
}

TEST(ResultTest, Unwrap) { EXPECT_EQ(result_ok.unwrap(), 42); }

TEST(ResultTest, UnwrapErr) { EXPECT_EQ(result_err.unwrap_err(), "error"); }
