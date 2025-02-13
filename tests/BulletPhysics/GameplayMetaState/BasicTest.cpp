
#include <gtest/gtest.h>

#include <BulletPhysics/GameplayMetaState/Basic.hpp>
#include <AllegroFlare/Testing/ErrorAssertions.hpp>


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


TEST(BulletPhysics_GameplayMetaState_BasicTest,
   set_state__when_setting_a_state_that_does_not_exist__will_throw_an_error)
{
   BulletPhysics::GameplayMetaState::Basic basic;
   EXPECT_THROW_WITH_MESSAGE(
      basic.set_state(9999),
      std::runtime_error,
      "[BulletPhysics::GameplayMetaState::Basic::set_state]: error: guard \"is_valid_state(state)\" not met"
   );
}


