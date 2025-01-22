
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
   //delete basic;
}


TEST(BulletPhysics_Examples_BasicTest, initialize__will_not_blow_up)
{
   BulletPhysics::Examples::Basic basic;
   basic.initialize();
   basic._destroy();
}


