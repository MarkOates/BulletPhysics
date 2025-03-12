#pragma once


#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/Camera2D.hpp>
#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/FontBin.hpp>
#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Placement3D.hpp>
#include <AllegroFlare/Screens/Gameplay.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <BulletPhysics/DynamicsWorld.hpp>
#include <BulletPhysics/Examples/Knockdown.hpp>
#include <BulletPhysics/GameplayMetaState/Basic.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <btBulletDynamicsCommon.h>
#include <string>
#include <utility>
#include <vector>


namespace BulletPhysics
{
   namespace Examples
   {
      class Knockdown : public AllegroFlare::Screens::Gameplay
      {
      private:
         std::string data_folder_path;
         std::vector<std::string> player_inventory_items;
         int player_num_lives;
         btDiscreteDynamicsWorld* dynamics_world;
         BulletPhysics::DynamicsWorld* dynamics_world_object;
         btRigidBody* sphere_body;
         float sphere_diameter;
         float dip_to_black_opacity;
         btVector3 sphere_initial_position;
         btCollisionShape* sphere_shape;
         btCollisionShape* cube_shape;
         std::vector<std::pair<btRigidBody*, btCollisionShape*>> cubes;
         std::vector<float> cube_initial_heights;
         AllegroFlare::Model3D* shape_model;
         std::vector<std::pair<btRigidBody*, btCollisionShape*>> shapes;
         btRigidBody* ground_body;
         btCollisionShape* ground_shape;
         bool player_has_thrown_ball;
         bool initialized;
         bool game_world_is_setup_and_requires_destruction;
         bool destroyed;
         BulletPhysics::GameplayMetaState::Basic gameplay_meta_state;
         AllegroFlare::Camera3D camera3d;
         AllegroFlare::Camera2D hud_camera;
         AllegroFlare::ModelBin model_bin;
         AllegroFlare::BitmapBin bitmap_bin;
         AllegroFlare::FontBin font_bin;
         void clear();
         btConvexHullShape* create_convex_shape(AllegroFlare::Model3D* model=nullptr);

      protected:


      public:
         Knockdown();
         virtual ~Knockdown();

         void set_data_folder_path(std::string data_folder_path);
         void set_player_inventory_items(std::vector<std::string> player_inventory_items);
         void set_player_num_lives(int player_num_lives);
         void set_shape_model(AllegroFlare::Model3D* shape_model);
         std::string get_data_folder_path() const;
         std::vector<std::string> get_player_inventory_items() const;
         int get_player_num_lives() const;
         int num_cubes();
         int num_shapes();
         virtual bool load_level_by_identifier(std::string possible_type="") override;
         void reset();
         void create_multiple_cubes();
         void create_stacked_cubes();
         void create_multiple_shapes();
         void create_shapes_from_tmj_file();
         BulletPhysics::Examples::Knockdown* create();
         void initialize();
         void launch_ball(btVector3* position_=nullptr, btVector3* velocity_=nullptr);
         bool is_cube_knocked_down(btRigidBody* cube=nullptr, float initial_height=0.0f, float threshold=0.8f);
         int count_cubes_knocked_down();
         std::pair<btRigidBody*, btCollisionShape*> create_dynamic_mesh_shape(AllegroFlare::Model3D* model=nullptr, float x=0.0f, float y=50.0f, float z=0.0f);
         void step_physics(double time_step=1.0 / 60.0);
         void output_body_positions_and_rotations(double time_step=1.0 / 60.0);
         void capture_sphere_body_position_and_rotation(AllegroFlare::Vec3D* position=nullptr, AllegroFlare::Vec3D* rotation_euler=nullptr);
         static void build_transform_bullet(ALLEGRO_TRANSFORM* transform=nullptr, AllegroFlare::Placement3D* placement_3d=nullptr);
         void capture_cube_body_position_and_rotation(AllegroFlare::Vec3D* position=nullptr, AllegroFlare::Vec3D* rotation_euler=nullptr, int cube_index=-1);
         void capture_shape_body_position_and_rotation(AllegroFlare::Vec3D* position=nullptr, AllegroFlare::Vec3D* rotation_euler=nullptr, int shape_index=-1);
         void capture_rigid_body_position_and_rotation(AllegroFlare::Vec3D* position=nullptr, AllegroFlare::Vec3D* rotation_euler=nullptr, btRigidBody* rigid_body=nullptr);
         virtual void destroy() override;
         void set_camera_to_start_position();
         void initialize_render();
         void destroy_render();
         virtual void primary_update_func(double time_now=al_get_time(), double time_step=1.0 / 60.0) override;
         void render_world();
         void render_hud();
         virtual void primary_render_func() override;
         ALLEGRO_FONT* get_any_font(AllegroFlare::FontBin* font_bin=nullptr, int size=-30);
         bool showing_final_score();
         bool showing_ready_banner();
         bool showing_gameplay_instructions();
         bool showing_press_any_key_to_continue_after_score_tally();
         bool waiting_for_player_input_to_continue();
         void advance_from_waiting_for_player_input_to_continue();
      };
   }
}



