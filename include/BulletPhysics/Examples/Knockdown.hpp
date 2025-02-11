#pragma once


#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/Placement3D.hpp>
#include <AllegroFlare/Screens/Gameplay.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <allegro5/allegro.h>
#include <btBulletDynamicsCommon.h>
#include <cstdint>
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
         enum State
         {
            STATE_UNDEF = 0,
            STATE_WAITING_FOR_PLAYER_TO_THROW_BALL,
            STATE_IN_SIMULATION,
            STATE_TALLYING_SCORE,
            STATE_SCORE_TALLIED,
         };
         std::string data_folder_path;
         btDefaultCollisionConfiguration collision_configuration;
         btCollisionDispatcher dispatcher;
         btDbvtBroadphase broadphase;
         btSequentialImpulseConstraintSolver solver;
         btDiscreteDynamicsWorld* dynamics_world;
         btRigidBody* sphere_body;
         float sphere_diameter;
         btVector3 sphere_initial_position;
         btCollisionShape* sphere_shape;
         btCollisionShape* cube_shape;
         std::vector<std::pair<btRigidBody*, btCollisionShape*>> cubes;
         std::vector<float> cube_initial_heights;
         AllegroFlare::Model3D* shape_model;
         std::vector<std::pair<btRigidBody*, btCollisionShape*>> shapes;
         btRigidBody* ground_body;
         btCollisionShape* ground_shape;
         bool initialized;
         bool destroyed;
         uint32_t state;
         bool state_is_busy;
         float state_changed_at;
         btConvexHullShape* create_convex_shape(AllegroFlare::Model3D* model=nullptr);

      protected:


      public:
         Knockdown();
         ~Knockdown();

         void set_data_folder_path(std::string data_folder_path);
         void set_shape_model(AllegroFlare::Model3D* shape_model);
         std::string get_data_folder_path() const;
         uint32_t get_state() const;
         int num_cubes();
         int num_shapes();
         void create_multiple_cubes();
         void create_stacked_cubes();
         void create_multiple_shapes();
         void create_shapes_from_tmj_file();
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
         void destroy();
         void primary_update_func(double time_now=al_get_time(), double time_step=1.0 / 60.0);
         void set_state(uint32_t state=STATE_UNDEF, bool override_if_busy=false);
         void update_state(float time_now=al_get_time());
         static bool is_valid_state(uint32_t state=STATE_UNDEF);
         bool is_state(uint32_t possible_state=STATE_UNDEF);
         float infer_current_state_age(float time_now=al_get_time());
         bool showing_final_score();
      };
   }
}



