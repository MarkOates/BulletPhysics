
#include <gtest/gtest.h>

#include <BulletPhysics/Examples/Cubes.hpp>
#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/Testing/WithInteractionFixture.hpp>
#include <AllegroFlare/Useful.hpp> // For draw_crosshair
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Placement3D.hpp>


class BulletPhysics_Examples_CubesTest : public ::testing::Test {};
class BulletPhysics_Examples_CubesTestWithInteractionFixture : public AllegroFlare::Testing::WithInteractionFixture {};



TEST_F(BulletPhysics_Examples_CubesTest, can_be_created_without_blowing_up)
{
   BulletPhysics::Examples::Cubes basic;
}



TEST_F(BulletPhysics_Examples_CubesTest, when_initialized_freed_and_not_destroyed__will_output_a_warning)
{
   BulletPhysics::Examples::Cubes *basic = new BulletPhysics::Examples::Cubes;
   basic->initialize();

   testing::internal::CaptureStdout();
   delete basic;
   std::string captured_cout = testing::internal::GetCapturedStdout();

   std::string expected_warning_message =
     "[1;33m[BulletPhysics::Examples::Cubes::~destroy]: warning: This instance is being destroyed but the "
     "destroy() method has not been called.[0m\n";

   EXPECT_EQ(expected_warning_message, captured_cout);
}



TEST_F(BulletPhysics_Examples_CubesTest, initialize__will_not_blow_up)
{
   BulletPhysics::Examples::Cubes basic;
   basic.initialize();
   basic.destroy();
}



TEST_F(BulletPhysics_Examples_CubesTest, step_physics__will_step_the_simulation)
{
   BulletPhysics::Examples::Cubes basic;
   basic.initialize();

   // Run simulation
   for (int i = 0; i < 150; i++)
   {
      // Make simulation step
      basic.step_physics(1.0 / 60.0);

      // Capture and output the physics information on the sphere
      basic.output_body_positions_and_rotations();
   }

   basic.destroy();
}



TEST_F(BulletPhysics_Examples_CubesTestWithInteractionFixture, will_work_with_the_expected_context)
{
   BulletPhysics::Examples::Cubes physics;
   AllegroFlare::Camera3D camera;
   AllegroFlare::ModelBin model_bin;
   model_bin.set_full_path(get_data_folder_path() + "models");

   AllegroFlare::Model3D *sphere_model = model_bin.auto_get("unit_sphere-01.obj");
   AllegroFlare::Model3D *cube_model = model_bin.auto_get("centered_unit_cube-02.obj");
   cube_model->set_texture(get_bitmap_bin_ref().auto_get("centered_unit_cube-03-faces.png"));

   // Initialize the physics
   physics.initialize();

   // Setup the camera
   camera.stepout = AllegroFlare::Vec3D(0, 2.25, 14);
   camera.tilt = 0.5;
   camera.spin = 0.125;
   camera.near_plane = 0.25;
   camera.far_plane = 500.0;

   // Create some scene "entity" variables
   AllegroFlare::Vec3D sphere_body_position;
   AllegroFlare::Vec3D cube_body_position;
   AllegroFlare::Placement3D sphere_body_placement;
   AllegroFlare::Placement3D cube_body_placement;

   while(interactive_test_wait_for_event())
   {
      ALLEGRO_EVENT &current_event = *interactive_test_get_current_event();

      switch(current_event.type)
      {
         case ALLEGRO_EVENT_TIMER:
         {
            // Step the physics
            physics.step_physics(1.0 / 60.0 * 2);

            // Synchronize the physics with the visual
            physics.capture_sphere_body_position_and_rotation(&sphere_body_position);
            physics.capture_cube_body_position_and_rotation(&cube_body_position);

            // Update the sphere and cube placements
            sphere_body_placement.position = sphere_body_position;
            cube_body_placement.position = cube_body_position;

            // Render
            clear();
            camera.setup_projection_on(al_get_target_bitmap());

            // Draw the sphere and cube positions with crosshairs
            AllegroFlare::draw_crosshair(sphere_body_position, ALLEGRO_COLOR{1, 1, 1, 1});
            AllegroFlare::draw_crosshair(cube_body_position, ALLEGRO_COLOR{1, 1, 1, 1});

            // Draw the sphere and cube objects
            sphere_body_placement.start_transform();
            sphere_model->draw();
            sphere_body_placement.restore_transform();

            cube_body_placement.start_transform();
            cube_model->draw();
            cube_body_placement.restore_transform();

            //cube_body_placement.position = cube_body_position;

            // Finish the interactive rendering
            interactive_test_render_status();
            al_flip_display();
         }
         break;

         //// For example:
         //case ALLEGRO_FLARE_EVENT_PLAY_SOUND_EFFECT:
         //{
            //std::cout << "[AllegroFlare_Elements_MultiListTestWithAllegroRenderingFixture]: INFO: "
                      //<< "Play sound effect event was emitted. "
                      //<< std::endl;
         //}
         //break;

         //// For example:
         //case ALLEGRO_EVENT_KEY_DOWN:
         //{
            //bool shift = current_event.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT;
            //switch(current_event.keyboard.keycode)
            //{
               //case ALLEGRO_KEY_ENTER:
                  //// Do something
               //break;
            //}
         //}
         //break;
      }
   }
}


