
#include <gtest/gtest.h>

#include <BulletPhysics/Examples/Knockdown.hpp>
#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/Camera2D.hpp>
#include <AllegroFlare/Testing/WithInteractionFixture.hpp>
#include <AllegroFlare/Testing/WithAllegroFlareFrameworksFullFixture.hpp>
#include <AllegroFlare/Useful.hpp> // For draw_crosshair
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Placement3D.hpp>


class BulletPhysics_Examples_KnockdownTest : public ::testing::Test {};
class BulletPhysics_Examples_KnockdownTestWithInteractionFixture
   : public AllegroFlare::Testing::WithInteractionFixture
{};
class BulletPhysics_Examples_KnockdownTestWithAllegroFlareFrameworksFullFixture
   : public AllegroFlare::Testing::WithAllegroFlareFrameworksFullFixture
{};



TEST_F(BulletPhysics_Examples_KnockdownTest, can_be_created_without_blowing_up)
{
   BulletPhysics::Examples::Knockdown basic;
}



TEST_F(BulletPhysics_Examples_KnockdownTest, DISABLED__when_initialized_freed_and_not_destroyed__will_output_a_warning)
{
   BulletPhysics::Examples::Knockdown *basic = new BulletPhysics::Examples::Knockdown;
   basic->initialize();

   testing::internal::CaptureStdout();
   delete basic;
   std::string captured_cout = testing::internal::GetCapturedStdout();

   std::string expected_warning_message =
     "[1;33m[BulletPhysics::Examples::Knockdown::~destroy]: warning: This instance is being destroyed but the "
     "destroy() method has not been called.[0m\n";

   EXPECT_EQ(expected_warning_message, captured_cout);
}



TEST_F(BulletPhysics_Examples_KnockdownTest, DISABLED__initialize__will_not_blow_up)
{
   BulletPhysics::Examples::Knockdown basic;
   basic.initialize();
   basic.destroy();
}



TEST_F(BulletPhysics_Examples_KnockdownTest, DISABLED__step_physics__will_step_the_simulation)
{
   BulletPhysics::Examples::Knockdown basic;
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



TEST_F(BulletPhysics_Examples_KnockdownTestWithInteractionFixture, CAPTURE__will_work_with_the_expected_context)
{
   //screen.set_data_folder_path(get_framework_data_folder_path());
   BulletPhysics::Examples::Knockdown screen;
   screen.set_data_folder_path(get_data_folder_path());
   AllegroFlare::Camera3D camera;
   AllegroFlare::Camera2D hud_camera;
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

   // Add pause behavior
   screen.set_on_paused_callback_func([](AllegroFlare::Screens::Gameplay* screen, void* user_data){
      // NOTE: Typically:
      // get_event_emitter()->emit_router_event(AllegroFlare::Routers::Standard::EVENT_PAUSE_GAME);
      screen->suspend_gameplay(); // TODO: Not sure if this is correct
   });

   // Initialize the physics
   screen.set_shape_model(shape_model);
   screen.initialize();
   screen.load_level_by_identifier("foo");

   // Setup the camera
   camera.stepout = AllegroFlare::Vec3D(0, 1.25, 16);
   camera.tilt = 0.65;
   camera.spin = 1.25;
   camera.near_plane = 0.25;
   camera.far_plane = 500.0;

   // Create some scene "entity" variables
   AllegroFlare::Vec3D sphere_body_position;
   //AllegroFlare::Vec3D cube_body_position;
   AllegroFlare::Vec3D sphere_body_rotation;
   AllegroFlare::Vec3D cube_body_rotation;
   AllegroFlare::Placement3D sphere_body_placement;
   AllegroFlare::Placement3D cube_body_placement;

   std::vector<AllegroFlare::Vec3D> cube_body_positions;
   std::vector<AllegroFlare::Vec3D> cube_body_rotations;
   std::vector<AllegroFlare::Placement3D> cube_body_placements;
   int num_cubes = screen.num_cubes();
   cube_body_positions.resize(num_cubes);
   cube_body_rotations.resize(num_cubes);
   cube_body_placements.resize(num_cubes);

   std::vector<AllegroFlare::Vec3D> shape_body_positions;
   std::vector<AllegroFlare::Vec3D> shape_body_rotations;
   std::vector<AllegroFlare::Placement3D> shape_body_placements;
   int num_shapes = screen.num_shapes();
   shape_body_positions.resize(num_shapes);
   shape_body_rotations.resize(num_shapes);
   shape_body_placements.resize(num_shapes);

   //int presteps = 60;
   //for (int i=0; i<presteps; i++)
   //{
      //screen.primary_update_func(al_get_time(), 1.0 / 60.0);
   //}


   while(interactive_test_wait_for_event())
   {
      ALLEGRO_EVENT &current_event = *interactive_test_get_current_event();

      switch(current_event.type)
      {
         case ALLEGRO_EVENT_KEY_DOWN: {
            screen.key_down_func(&current_event);
         } break;

         case ALLEGRO_EVENT_TIMER:
         {
            // Step the physics
            //physics.step_physics(1.0 / 60.0 * 2.0f); // * 0.125);
            double step_duration = 1.0 / 60.0 * 2.0;
            screen.primary_update_func(al_get_time(), step_duration);
            camera.spin -= 0.0125 * 0.25;

            // Synchronize the physics with the visual
            screen.capture_sphere_body_position_and_rotation(&sphere_body_position, &sphere_body_rotation);
            //screen.capture_cube_body_position_and_rotation(&cube_body_position, &cube_body_rotation);
            //cube_body_placement.scale = {2, 2, 2};
            for (int i=0; i<num_cubes; i++)
            {
               screen.capture_cube_body_position_and_rotation(&cube_body_positions[i], &cube_body_rotations[i], i);
               cube_body_placements[i].position = cube_body_positions[i];
               cube_body_placements[i].rotation = cube_body_rotations[i];
               //cube_body_placements[i].scale = {2, 2, 2};
            }

            for (int i=0; i<num_shapes; i++)
            {
               screen.capture_shape_body_position_and_rotation(&shape_body_positions[i], &shape_body_rotations[i], i);
               shape_body_placements[i].position = shape_body_positions[i];
               shape_body_placements[i].rotation = shape_body_rotations[i];
               //cube_body_placements[i].scale = {2, 2, 2};
            }
            

            // Update the sphere and cube placements
            sphere_body_placement.position = sphere_body_position;
            sphere_body_placement.rotation = sphere_body_rotation;
            sphere_body_placement.scale = { 2, 2, 2 };
            //cube_body_placement.position = cube_body_position;
            //cube_body_placement.rotation = cube_body_rotation;

            // Render
            clear();
            camera.setup_projection_on(al_get_target_bitmap());

            // Draw the sphere and cube positions with crosshairs
            //AllegroFlare::draw_crosshair(sphere_body_position, ALLEGRO_COLOR{1, 1, 1, 1});
            //AllegroFlare::draw_crosshair(cube_body_position, ALLEGRO_COLOR{1, 1, 1, 1});

            // Draw the sphere and cube objects
            sphere_body_placement.start_transform();
            sphere_model->draw();
            sphere_body_placement.restore_transform();


            /*
            {
               ALLEGRO_TRANSFORM previous_transform;

               if (!al_get_current_transform()) return;
               al_copy_transform(&previous_transform, al_get_current_transform());

               //this->build_transform(&transform);

               //al_compose_transform(&transform, &previous_transform);
               //al_use_transform(&transform);
               
               ALLEGRO_TRANSFORM transform;
               BulletPhysics::Examples::Knockdown::build_transform_bullet(&transform, &cube_body_placement);
               al_compose_transform(&transform, &previous_transform);
               al_use_transform(&transform);
               //cube_body_placement.start_transform();


               cube_model->draw();


               //cube_body_placement.restore_transform();
               al_use_transform(&previous_transform);
            }
            */


            int num_cubes = screen.num_cubes();
            for (int i=0; i<num_cubes; i++)
            {
               ALLEGRO_TRANSFORM previous_transform;

               if (!al_get_current_transform()) return;
               al_copy_transform(&previous_transform, al_get_current_transform());

               //this->build_transform(&transform);

               //al_compose_transform(&transform, &previous_transform);
               //al_use_transform(&transform);
               
               ALLEGRO_TRANSFORM transform;
               BulletPhysics::Examples::Knockdown::build_transform_bullet(&transform, &cube_body_placements[i]);
               al_compose_transform(&transform, &previous_transform);
               al_use_transform(&transform);
               //cube_body_placement.start_transform();


               cube_model->draw();


               //cube_body_placement.restore_transform();
               al_use_transform(&previous_transform);
            }


            int num_shapes = screen.num_shapes();
            for (int i=0; i<num_shapes; i++)
            {
               ALLEGRO_TRANSFORM previous_transform;

               if (!al_get_current_transform()) return;
               al_copy_transform(&previous_transform, al_get_current_transform());

               //this->build_transform(&transform);

               //al_compose_transform(&transform, &previous_transform);
               //al_use_transform(&transform);
               
               ALLEGRO_TRANSFORM transform;
               BulletPhysics::Examples::Knockdown::build_transform_bullet(&transform, &shape_body_placements[i]);
               al_compose_transform(&transform, &previous_transform);
               al_use_transform(&transform);
               //shape_body_placement.start_transform();


               shape_model->draw();


               //shape_body_placement.restore_transform();
               al_use_transform(&previous_transform);
            }


            //cube_body_placement.position = cube_body_position;
            //camera.setup_projection_on(al_get_target_bitmap());
            //hud_camera.setup_projection_on(al_get_target_bitmap());
            int num_cubes_knocked_down = screen.count_cubes_knocked_down();
            hud_camera.setup_dimensional_projection(al_get_target_bitmap());

            ALLEGRO_FONT *font = get_any_font();
            al_draw_multiline_textf(
               font,
               ALLEGRO_COLOR{1, 1, 1, 1},
               1920/2,
               80,
               1920,
               al_get_font_line_height(font),
               ALLEGRO_ALIGN_CENTER,
               "KNOCKED DOWN\n%d",
                  num_cubes_knocked_down
            );


               ALLEGRO_FONT *font_x = get_font_bin_ref().auto_get("Oswald-Medium.ttf -262");
            if (screen.showing_final_score())
            {
               ALLEGRO_FONT *font = font_x;
               al_draw_multiline_textf(
                  font,
                  ALLEGRO_COLOR{0.4, 0.72, 0.8, 1},
                  1920/2,
                  1080/2 - al_get_font_line_height(font),
                  1920,
                  al_get_font_line_height(font),
                  ALLEGRO_ALIGN_CENTER,
                  "KNOCKED DOWN\n%d",
                     num_cubes_knocked_down
               );
            }



            // For debugging, show state
            //al_draw_multiline_textf(
               //font,
               //ALLEGRO_COLOR{1, 1, 1, 1},
               //1920/2,
               //1080-80*2,
               //1920,
               //al_get_font_line_height(font),
               //ALLEGRO_ALIGN_CENTER,
               //"STATE: %d",
                  //screen.get_state()
            //);


            
             

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



#include <AllegroFlare/Routers/Standard.hpp>

TEST_F(BulletPhysics_Examples_KnockdownTestWithAllegroFlareFrameworksFullFixture,
   INTERACTIVE__will_work_in_a_Frameworks_Full_context)
{
   BulletPhysics::Examples::Knockdown *screen = new BulletPhysics::Examples::Knockdown();
   screen->set_data_folder_path(get_framework_data_folder_path());
   screen->set_on_paused_callback_func([this](AllegroFlare::Screens::Gameplay* screen, void* user_data){
      get_framework_event_emitter()->emit_router_event(AllegroFlare::Routers::Standard::EVENT_PAUSE_GAME);
   });
   screen->initialize();
   screen->load_level_by_identifier("foo");

   framework_register_and_activate_screen("screen", screen);

   framework_run_loop(-12);

   screen->destroy();
}



/*
TEST_F(BulletPhysics_Examples_KnockdownTestWithAllegroFlareFrameworksFullFixture,
   INTERACTIVE__will_work_in_a_Frameworks_Full_context)
{
   BulletPhysics::Examples::Knockdown *screen = new BulletPhysics::Examples::Knockdown();
   screen->set_data_folder_path(get_framework_data_folder_path());
   screen->set_on_paused_callback_func([this](AllegroFlare::Screens::Gameplay* screen, void* user_data){
      get_framework_event_emitter()->emit_router_event(AllegroFlare::Routers::Standard::EVENT_PAUSE_GAME);
   });
   screen->initialize();
   screen->load_level_by_identifier("foo");

   framework_register_and_activate_screen("screen", screen);

   framework_run_loop(-12);

   screen->destroy();
}
*/



/*
TEST_F(Krampus24_Gameplay_ScreenTestWithAllegroFrameworksFullFixture,
   CAPTURE__TIMED_INTERACTIVE__will_run_as_expected)
{
   // Define our level
   //std::string collision_mesh_identifier = "some_level-03-bumps.obj";
   //std::string visual_mesh_identifier = "a_masonry_floor-01.obj";
   //std::string visual_mesh_texture_identifier = "tiles_dungeon_v1.1-1_to_1.png";
   //std::string visual_mesh_texture_identifier = "bark.jpg";


   //std::string collision_mesh_identifier = "basic_baking-02-collision_mesh.obj";
   //std::string visual_mesh_identifier = "basic_baking-02-visual.obj";
   //std::string visual_mesh_texture_identifier = "basic_baking-01-baked_shadow-reduced.jpg";
   //std::string blocking_filename = "basic_baking-02.blocking";

   //std::string collision_mesh_identifier = "tree-03-collision_mesh.obj";
   //std::string visual_mesh_identifier = "tree-03-visual.obj";
   //std::string visual_mesh_texture_identifier = "RETRO_TEXTURE_PACK_V15-atlas-01.png";
   //std::string blocking_filename = "tree-03.blocking";


   //std::string visual_mesh_texture_identifier = "basic_baking-01-baked_shadow.png";

   //std::string visual_mesh_identifier = "basic_baking-01-visual.obj";
   //std::string collision_mesh_identifier = "station2-01-collision_mesh.obj";
   //std::string collision_mesh_identifier = "meshfps_level-03-collision_mesh.obj";
   //std::string visual_mesh_identifier = "";
   //std::string visual_mesh_texture_identifier = "";
   //std::string visual_mesh_identifier = "meshfps_level-02.obj";

   //std::string visual_mesh_identifier = "meshfps_level-03-visual.obj";
   //std::string visual_mesh_texture_identifier = "RETRO_TEXTURE_PACK_V15-atlas-resave.png";

   //AllegroFlare::DeploymentEnvironment deployment_environment("test");

   // Setup the screen
   TestGameConfiguration game_configuration;
   Krampus24::Gameplay::Screen screen;
   screen.set_data_folder_path(get_framework_data_folder_path());
   screen.set_audio_controller(get_framework_audio_controller());
   screen.set_event_emitter(get_framework_event_emitter());
   screen.set_bitmap_bin(get_framework_bitmap_bin());
   screen.set_font_bin(get_framework_font_bin());
   screen.set_model_bin(get_framework_model_bin());
   screen.set_is_deployment_environment_production(false); // HACK
   screen.set_dialog_system(get_framework_dialog_system());
   screen.set_game_configuration(&game_configuration);
   screen.set_build_scripting_instance_func(
      [this](Krampus24::Gameplay::Screen* screen) -> Krampus24::Gameplay::ScriptingInterface* {

      //std::cout << "---- ****** inside building scripting ----" << std::endl;
      // Build a scripting
      //Krampus24::Game::Scripting::JumpingWorld *scripting = new Krampus24::Game::Scripting::JumpingWorld;
      Atratus::Game::Scripting::MultiSceneWorld *scripting = new Atratus::Game::Scripting::MultiSceneWorld;
      scripting->set_data_folder_path(get_framework_data_folder_path());
      scripting->set_entities(&screen->get_entities_ref());
      scripting->set_collision_observer(&screen->get_collision_observer_ref());
      scripting->set_font_bin(screen->get_font_bin());
      scripting->set_dialog_system(screen->get_dialog_system());
      scripting->set_event_emitter(screen->get_event_emitter());
      scripting->initialize();

      return scripting;
   });

   //screen.set_collision_mesh_identifier(collision_mesh_identifier);
   //screen.set_visual_mesh_identifier(visual_mesh_identifier);
   //screen.set_visual_mesh_texture_identifier(visual_mesh_texture_identifier);
   //screen.set_blocking_filename(blocking_filename);
   screen.set_on_finished_callback_func([](AllegroFlare::Screens::Gameplay*, void*){
      std::cout << "-- inside provided on_finished_callback_func()" << std::endl;
      std::cout << "*** GAME IS COMPLETED ***" << std::endl;
      //throw std::runtime_error("Game is completed.");
   });
   screen.initialize();

   // Build a map
   //AllegroFlare::Physics::CollisionMesh *collision_mesh = new AllegroFlare::Physics::CollisionMesh();
   //collision_mesh->set_model(get_framework_model_bin()->operator[](collision_mesh_identifier));
   //collision_mesh->load();
   //screen.set_collision_mesh(collision_mesh);

   // Create some entities
   //AllegroFlare::Vec3D player_spawn_position = {2, 0.001, -2};
   
   //Krampus24::Gameplay::Entities::Base* player_entity =
      //new Krampus24::Gameplay::Entities::Base();
   //player_entity->get_placement_ref().size = {0.5, 0.5, 0.5};
   //player_entity->get_placement_ref().position = player_spawn_position;
   //player_entity->get_placement_ref().align = {0.5, 0.5, 0.5}; // Align has no effect in this case
   //std::vector<Krampus24::Gameplay::Entities::Base*> &entities = screen.get_entities_ref();
   //entities.push_back(player_entity); // Player entity
   //entities.push_back(build_entity(AllegroFlare::Vec3D(3, 2, -8)));
   //entities.push_back(build_entity(AllegroFlare::Vec3D(6, 4, -9)));
   //entities.push_back(build_entity(AllegroFlare::Vec3D(16, 4, -9)));
   //entities.push_back(build_entity(AllegroFlare::Vec3D(-3, 2, -8), false));
   //screen.set_entities(&entities);

   // Set the visual mesh
   //AllegroFlare::Model3D *visual_mesh = get_framework_model_bin()->operator[](visual_mesh_identifier);
   //visual_mesh->set_texture(get_framework_bitmap_bin()->operator[](visual_mesh_texture_identifier));
   //screen.set_visual_mesh(visual_mesh);

   // Set the player spawn position
   //screen.set_player_spawn_position(player_spawn_position);


   framework_register_and_activate_screen("screen", &screen);

   framework_run_loop(-12);
}
*/



