
#include <gtest/gtest.h>

#include <AllegroFlare/PlaytimeTracker.hpp>


TEST(AllegroFlare_PlaytimeTrackerTest, can_be_created_without_blowing_up)
{
   AllegroFlare::PlaytimeTracker playtime_tracker;
}


TEST(AllegroFlare_PlaytimeTrackerTest, formatted_current_total_playtime_hms__will_return_the_expected_formatted_string)
{
   AllegroFlare::PlaytimeTracker playtime_tracker;
   playtime_tracker.from_string("837456");

   std::string expected_time_str = "13m 57s";
   std::string actual_time_str = playtime_tracker.formatted_current_total_playtime_hms();
   EXPECT_EQ(expected_time_str, actual_time_str);
}


