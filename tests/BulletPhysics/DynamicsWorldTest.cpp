
#include <gtest/gtest.h>

#include <BulletPhysics/DynamicsWorld.hpp>
#include <AllegroFlare/Testing/ErrorAssertions.hpp>


TEST(BulletPhysics_DynamicsWorldTest, can_be_created_without_blowing_up)
{
   BulletPhysics::DynamicsWorld dynamics_world;
}


TEST(BulletPhysics_DynamicsWorldTest, can_be_initialized_without_blowing_up)
{
   BulletPhysics::DynamicsWorld dynamics_world;
   dynamics_world.initialize();
}


TEST(BulletPhysics_DynamicsWorldTest, destroy__after_initialized__will_not_blow_up)
{
   BulletPhysics::DynamicsWorld dynamics_world;
   dynamics_world.initialize();
   dynamics_world.destroy();
}


TEST(BulletPhysics_DynamicsWorldTest, at_destruction__after_initialized_and_not_destroyed__will_output_a_warning)
{
   BulletPhysics::DynamicsWorld *dynamics_world = new BulletPhysics::DynamicsWorld();
   dynamics_world->initialize();
   std::string expected_warning_message = "\x1B[1;33m[BulletPhysics::DynamicsWorld::~destroy]: warning: This "
      "instance is being destroyed but the destroy() method has not been called.\x1B[0m\n";
   EXPECT_STDOUT(
      delete dynamics_world,
      expected_warning_message
   )
}


