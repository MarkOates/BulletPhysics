
#include <gtest/gtest.h>

#include <BulletPhysics/GameProgressAndStateInfo.hpp>


TEST(BulletPhysics_GameProgressAndStateInfoTest, can_be_created_without_blowing_up)
{
   BulletPhysics::GameProgressAndStateInfo game_progress_and_state_info;
}


TEST(BulletPhysics_GameProgressAndStateInfoTest, TYPE__has_the_expected_value)
{
   EXPECT_STREQ(
     "BulletPhysics/GameProgressAndStateInfo",
     BulletPhysics::GameProgressAndStateInfo::TYPE
   );
}


TEST(BulletPhysics_GameProgressAndStateInfoTest, type__has_the_expected_value_matching_TYPE)
{
   BulletPhysics::GameProgressAndStateInfo game_progress_and_state_info;
   EXPECT_EQ(BulletPhysics::GameProgressAndStateInfo::TYPE, game_progress_and_state_info.get_type());
}


