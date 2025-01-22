
#include <gtest/gtest.h>

#include <BulletPhysics/Examples/Basic.hpp>


TEST(BulletPhysics_Examples_BasicTest, can_be_created_without_blowing_up)
{
   BulletPhysics::Examples::Basic basic;
}


TEST(BulletPhysics_Examples_BasicTest, run__returns_the_expected_response)
{
   BulletPhysics::Examples::Basic basic;
   std::string expected_string = "Hello World!";
   EXPECT_EQ(expected_string, basic.run());
}


