
#include <gtest/gtest.h>

#include <BulletPhysics/Examples/Mesh.hpp>
#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/Testing/WithInteractionFixture.hpp>
#include <AllegroFlare/Useful.hpp> // For draw_crosshair
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Placement3D.hpp>


class BulletPhysics_Examples_MeshTest : public ::testing::Test {};
class BulletPhysics_Examples_MeshTestWithInteractionFixture : public AllegroFlare::Testing::WithInteractionFixture {};



TEST_F(BulletPhysics_Examples_MeshTest, can_be_created_without_blowing_up)
{
   BulletPhysics::Examples::Mesh basic;
}



TEST_F(BulletPhysics_Examples_MeshTest, DISABLED__when_initialized_freed_and_not_destroyed__will_output_a_warning)
{
   BulletPhysics::Examples::Mesh *basic = new BulletPhysics::Examples::Mesh;
   basic->initialize();

   testing::internal::CaptureStdout();
   delete basic;
   std::string captured_cout = testing::internal::GetCapturedStdout();

   std::string expected_warning_message =
     "[1;33m[BulletPhysics::Examples::Mesh::~destroy]: warning: This instance is being destroyed but the "
     "destroy() method has not been called.[0m\n";

   EXPECT_EQ(expected_warning_message, captured_cout);
}



TEST_F(BulletPhysics_Examples_MeshTest, DISABLED__initialize__will_not_blow_up)
{
   BulletPhysics::Examples::Mesh basic;
   basic.initialize();
   basic.destroy();
}



TEST_F(BulletPhysics_Examples_MeshTest, DISABLED__step_physics__will_step_the_simulation)
{
   BulletPhysics::Examples::Mesh basic;
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



TEST_F(BulletPhysics_Examples_MeshTestWithInteractionFixture, CAPTURE__will_work_with_the_expected_context)
{
   BulletPhysics::Examples::Mesh physics;
   AllegroFlare::Camera3D camera;
   AllegroFlare::ModelBin model_bin;
   model_bin.set_full_path(get_data_folder_path() + "models");

   AllegroFlare::Model3D *sphere_model = model_bin.auto_get("unit_sphere-01.obj");
   AllegroFlare::Model3D *cube_model = model_bin.auto_get("centered_unit_cube-03.obj");
   //AllegroFlare::Model3D *shape_model = model_bin.auto_get("obscure_unit_tetrahedron-02.obj");
   AllegroFlare::Model3D *shape_model = model_bin.auto_get("fancy_dice-01.obj");
   //AllegroFlare::Model3D *shape_model = model_bin.auto_get("dice_v-01.obj");
   cube_model->set_texture(get_bitmap_bin_ref().auto_get("dice_faces-04.png"));
   //shape_model->set_texture(get_bitmap_bin_ref().auto_get("obscure_unit_tetrahedron-02.png"));
   shape_model->set_texture(get_bitmap_bin_ref().auto_get("fancy_dice-02.png"));
   shape_model->set_texture(get_bitmap_bin_ref().auto_get("fancy_neopets_dice-01.png"));
   shape_model->set_texture(get_bitmap_bin_ref().auto_get("6_dice_sides-01b.png"));


   // Initialize the physics
   physics.set_shape_model(shape_model);
   physics.initialize();

   // Setup the camera
   camera.stepout = AllegroFlare::Vec3D(0, 1.25, 16);
   camera.tilt = 0.65;
   camera.spin = 1.25;
   camera.near_plane = 0.25;
   camera.far_plane = 500.0;

   // Create some scene "entity" variables
   AllegroFlare::Vec3D sphere_body_position;
   AllegroFlare::Vec3D cube_body_position;
   AllegroFlare::Vec3D sphere_body_rotation;
   AllegroFlare::Vec3D cube_body_rotation;
   AllegroFlare::Placement3D sphere_body_placement;
   AllegroFlare::Placement3D cube_body_placement;

   std::vector<AllegroFlare::Vec3D> cube_body_positions;
   std::vector<AllegroFlare::Vec3D> cube_body_rotations;
   std::vector<AllegroFlare::Placement3D> cube_body_placements;
   int num_cubes = physics.num_cubes();
   cube_body_positions.resize(num_cubes);
   cube_body_rotations.resize(num_cubes);
   cube_body_placements.resize(num_cubes);

   std::vector<AllegroFlare::Vec3D> shape_body_positions;
   std::vector<AllegroFlare::Vec3D> shape_body_rotations;
   std::vector<AllegroFlare::Placement3D> shape_body_placements;
   int num_shapes = physics.num_shapes();
   shape_body_positions.resize(num_shapes);
   shape_body_rotations.resize(num_shapes);
   shape_body_placements.resize(num_shapes);

   int presteps = 60;
   for (int i=0; i<presteps; i++)
   {
      physics.step_physics(1.0 / 60.0);
   }

   while(interactive_test_wait_for_event())
   {
      ALLEGRO_EVENT &current_event = *interactive_test_get_current_event();

      switch(current_event.type)
      {
         case ALLEGRO_EVENT_TIMER:
         {
            // Step the physics
            physics.step_physics(1.0 / 60.0 * 2.0f); // * 0.125);
            camera.spin -= 0.0125 * 0.25;

            // Synchronize the physics with the visual
            physics.capture_sphere_body_position_and_rotation(&sphere_body_position, &sphere_body_rotation);
            physics.capture_cube_body_position_and_rotation(&cube_body_position, &cube_body_rotation);
            //cube_body_placement.scale = {2, 2, 2};
            for (int i=0; i<num_cubes; i++)
            {
               physics.capture_cube_body_position_and_rotation(&cube_body_positions[i], &cube_body_rotations[i], i);
               cube_body_placements[i].position = cube_body_positions[i];
               cube_body_placements[i].rotation = cube_body_rotations[i];
               //cube_body_placements[i].scale = {2, 2, 2};
            }

            for (int i=0; i<num_shapes; i++)
            {
               physics.capture_shape_body_position_and_rotation(&shape_body_positions[i], &shape_body_rotations[i], i);
               shape_body_placements[i].position = shape_body_positions[i];
               shape_body_placements[i].rotation = shape_body_rotations[i];
               //cube_body_placements[i].scale = {2, 2, 2};
            }
            

            // Update the sphere and cube placements
            sphere_body_placement.position = sphere_body_position;
            sphere_body_placement.rotation = sphere_body_rotation;
            sphere_body_placement.scale = { 2, 2, 2 };
            cube_body_placement.position = cube_body_position;
            cube_body_placement.rotation = cube_body_rotation;

            // Render
            clear();
            camera.setup_projection_on(al_get_target_bitmap());

            // Draw the sphere and cube positions with crosshairs
            //AllegroFlare::draw_crosshair(sphere_body_position, ALLEGRO_COLOR{1, 1, 1, 1});
            //AllegroFlare::draw_crosshair(cube_body_position, ALLEGRO_COLOR{1, 1, 1, 1});

            // Draw the sphere and cube objects
            sphere_body_placement.start_transform();
            //sphere_model->draw();
            sphere_body_placement.restore_transform();


            {
               ALLEGRO_TRANSFORM previous_transform;

               if (!al_get_current_transform()) return;
               al_copy_transform(&previous_transform, al_get_current_transform());

               //this->build_transform(&transform);

               //al_compose_transform(&transform, &previous_transform);
               //al_use_transform(&transform);
               
               ALLEGRO_TRANSFORM transform;
               BulletPhysics::Examples::Mesh::build_transform_bullet(&transform, &cube_body_placement);
               al_compose_transform(&transform, &previous_transform);
               al_use_transform(&transform);
               //cube_body_placement.start_transform();


               cube_model->draw();


               //cube_body_placement.restore_transform();
               al_use_transform(&previous_transform);
            }


            int num_cubes = physics.num_cubes();
            for (int i=0; i<num_cubes; i++)
            {
               ALLEGRO_TRANSFORM previous_transform;

               if (!al_get_current_transform()) return;
               al_copy_transform(&previous_transform, al_get_current_transform());

               //this->build_transform(&transform);

               //al_compose_transform(&transform, &previous_transform);
               //al_use_transform(&transform);
               
               ALLEGRO_TRANSFORM transform;
               BulletPhysics::Examples::Mesh::build_transform_bullet(&transform, &cube_body_placements[i]);
               al_compose_transform(&transform, &previous_transform);
               al_use_transform(&transform);
               //cube_body_placement.start_transform();


               cube_model->draw();


               //cube_body_placement.restore_transform();
               al_use_transform(&previous_transform);
            }


            int num_shapes = physics.num_shapes();
            for (int i=0; i<num_shapes; i++)
            {
               ALLEGRO_TRANSFORM previous_transform;

               if (!al_get_current_transform()) return;
               al_copy_transform(&previous_transform, al_get_current_transform());

               //this->build_transform(&transform);

               //al_compose_transform(&transform, &previous_transform);
               //al_use_transform(&transform);
               
               ALLEGRO_TRANSFORM transform;
               BulletPhysics::Examples::Mesh::build_transform_bullet(&transform, &shape_body_placements[i]);
               al_compose_transform(&transform, &previous_transform);
               al_use_transform(&transform);
               //shape_body_placement.start_transform();


               shape_model->draw();


               //shape_body_placement.restore_transform();
               al_use_transform(&previous_transform);
            }


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


