
#include <gtest/gtest.h>

#include <BulletPhysics/GameplayMetaState/Base.hpp>


class GameplayMetaStateBaseTestClass : public BulletPhysics::GameplayMetaState::Base
{
public:
   GameplayMetaStateBaseTestClass()
      : BulletPhysics::GameplayMetaState::Base("GameplayMetaStateBaseTestClass")
   {}
};


TEST(BulletPhysics_GameplayMetaState_BaseTest, can_be_created_without_blowing_up)
{
   BulletPhysics::GameplayMetaState::Base base;
}


TEST(BulletPhysics_GameplayMetaState_BaseTest, TYPE__has_the_expected_value)
{
   EXPECT_STREQ(
     "BulletPhysics/GameplayMetaState/Base",
     BulletPhysics::GameplayMetaState::Base::TYPE
   );
}


TEST(BulletPhysics_GameplayMetaState_BaseTest, type__has_the_expected_value_matching_TYPE)
{
   BulletPhysics::GameplayMetaState::Base base;
   EXPECT_EQ(BulletPhysics::GameplayMetaState::Base::TYPE, base.get_type());
}


TEST(BulletPhysics_GameplayMetaState_BaseTest, derived_classes_will_have_the_expected_type)
{
   GameplayMetaStateBaseTestClass test_class;
   EXPECT_EQ("GameplayMetaStateBaseTestClass", test_class.get_type());
}


