#pragma once


#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/Placement3D.hpp>
#include <AllegroFlare/Screens/Gameplay.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <allegro5/allegro.h>
#include <btBulletDynamicsCommon.h>
#include <utility>
#include <vector>


namespace BulletPhysics
{
   namespace Examples
   {
      class Knockdown : public AllegroFlare::Screens::Gameplay
      {
      private:
         btDefaultCollisionConfiguration collision_configuration;
         btCollisionDispatcher dispatcher;
         btDbvtBroadphase broadphase;
         btSequentialImpulseConstraintSolver solver;
         btDiscreteDynamicsWorld* dynamics_world;
         btRigidBody* sphere_body;
         btCollisionShape* sphere_shape;
         btCollisionShape* cube_shape;
         std::vector<std::pair<btRigidBody*, btCollisionShape*>> cubes;
         AllegroFlare::Model3D* shape_model;
         std::vector<std::pair<btRigidBody*, btCollisionShape*>> shapes;
         btRigidBody* ground_body;
         btCollisionShape* ground_shape;
         bool initialized;
         bool destroyed;
         btConvexHullShape* create_convex_shape(AllegroFlare::Model3D* model=nullptr);

      protected:


      public:
         Knockdown();
         ~Knockdown();

         void set_shape_model(AllegroFlare::Model3D* shape_model);
         int num_cubes();
         int num_shapes();
         void initialize();
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
      };
   }
}



