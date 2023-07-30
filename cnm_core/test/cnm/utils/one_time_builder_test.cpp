#include <gtest/gtest.h>

#include <cnm/utils/one_time_builder.hpp>

struct TestObject {
  explicit TestObject(int v) : value{v} {}
  int value;
};

class OneTimeBuilderTest : public ::testing::Test {
 protected:
  Utils::OneTimeBuilder<TestObject> builder;
};

TEST_F(OneTimeBuilderTest, BuildOnce) {
  auto result = builder.build(20);

  EXPECT_TRUE(result.isOk());

  auto object = result.unwrap();
  EXPECT_EQ(object.value, 20);
}

TEST_F(OneTimeBuilderTest, BuildTwice) {
  auto result1 = builder.build(42);
  EXPECT_TRUE(result1.isOk());

  auto result2 = builder.build(24);
  EXPECT_FALSE(result2.isOk());
  EXPECT_EQ(result2.unwrapErr(), "object has been already created");
}