
#include <gtest/gtest.h>

#include <BulletPhysics/GameplayMetaState/Basic.hpp>


TEST(BulletPhysics_GameplayMetaState_BasicTest, can_be_created_without_blowing_up)
{
   BulletPhysics::GameplayMetaState::Basic basic;
}


TEST(BulletPhysics_GameplayMetaState_BasicTest, TYPE__has_the_expected_value)
{
   EXPECT_STREQ(
     "BulletPhysics/GameplayMetaState/Basic",
     BulletPhysics::GameplayMetaState::Basic::TYPE
   );
}


TEST(BulletPhysics_GameplayMetaState_BasicTest, type__has_the_expected_value_matching_TYPE)
{
   BulletPhysics::GameplayMetaState::Basic basic;
   EXPECT_EQ(BulletPhysics::GameplayMetaState::Basic::TYPE, basic.get_type());
}


