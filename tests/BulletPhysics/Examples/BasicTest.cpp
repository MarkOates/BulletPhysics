
#include <gtest/gtest.h>

#include <BulletPhysics/Examples/Basic.hpp>


TEST(BulletPhysics_Examples_BasicTest, can_be_created_without_blowing_up)
{
   BulletPhysics::Examples::Basic basic;
}


TEST(BulletPhysics_Examples_BasicTest, when_initialized_freed_and_not_destroyed__will_output_a_warning)
{
   BulletPhysics::Examples::Basic *basic = new BulletPhysics::Examples::Basic;
   basic->initialize();

   testing::internal::CaptureStdout();
   delete basic;
   std::string captured_cout = testing::internal::GetCapturedStdout();

   std::string expected_warning_message =
     "\x1B[1;33m[BulletPhysics::Examples::Basic::~destroy]: warning: This instance is being destroyed but the "
     "destroy() method has not been called.\x1B[0m\n";

   EXPECT_EQ(expected_warning_message, captured_cout);
}


TEST(BulletPhysics_Examples_BasicTest, initialize__will_not_blow_up)
{
   BulletPhysics::Examples::Basic basic;
   basic.initialize();
   basic.destroy();
}


