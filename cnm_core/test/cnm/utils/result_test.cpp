#include <gtest/gtest.h>

#include <cnm/utils/result.hpp>

using namespace Utils;

auto result_ok = Result<int, std::string>::Ok(42);
auto result_err = Result<int, std::string>::Err("error");

TEST(ResultTest, IsOk) {
  EXPECT_TRUE(result_ok.isOk());
  EXPECT_FALSE(result_ok.isErr());
  EXPECT_FALSE(result_err.isOk());
  EXPECT_TRUE(result_err.isErr());
}

TEST(ResultTest, Unwrap) { EXPECT_EQ(result_ok.unwrap(), 42); }

TEST(ResultTest, UnwrapErr) { EXPECT_EQ(result_err.unwrapErr(), "error"); }
