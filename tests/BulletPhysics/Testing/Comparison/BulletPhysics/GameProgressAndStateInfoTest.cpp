
#include <gtest/gtest.h>

#include <BulletPhysics/Testing/Comparison/BulletPhysics/GameProgressAndStateInfo.hpp>


TEST(AllegroFlare_Testing_Comparison_BulletPhysics_GameProgressAndStateInfoTest,
   PrintTo__with_an_AllegroFlare_GameProgressAndStateInfo__will_output_as_expected)
{
   std::stringstream ss;
   BulletPhysics::GameProgressAndStateInfo object;
   object.set_player_inventory_items({ "fruit", "veggie", "fork" });

   PrintTo(object, &ss);

   std::string expected_output =
      "GameProgressAndStateInfo(player_inventory_items: {\"fruit\", \"veggie\", \"fork\", }, )";
   std::string actual_output = ss.str();
   EXPECT_EQ(expected_output, actual_output);
}


TEST(AllegroFlare_Testing_Comparison_BulletPhysics_GameProgressAndStateInfoTest,
   equality_operator__works_with_google_test_EXPECT_statement)
{
   BulletPhysics::GameProgressAndStateInfo object;
   BulletPhysics::GameProgressAndStateInfo other_object;

   EXPECT_EQ(object, other_object);
}


