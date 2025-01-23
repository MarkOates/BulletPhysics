#pragma once


#include <AllegroFlare/Placement3D.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <allegro5/allegro.h>
#include <btBulletDynamicsCommon.h>
#include <utility>
#include <vector>


namespace BulletPhysics
{
   namespace Examples
   {
      class Cubes
      {
      private:
         btDefaultCollisionConfiguration collision_configuration;
         btCollisionDispatcher dispatcher;
         btDbvtBroadphase broadphase;
         btSequentialImpulseConstraintSolver solver;
         btDiscreteDynamicsWorld* dynamics_world;
         btRigidBody* sphere_body;
         btCollisionShape* sphere_shape;
         btRigidBody* cube_body;
         btCollisionShape* cube_shape;
         std::vector<std::pair<btRigidBody*, btCollisionShape*>> cubes;
         btRigidBody* ground_body;
         btCollisionShape* ground_shape;
         bool initialized;
         bool destroyed;

      protected:


      public:
         Cubes();
         ~Cubes();

         int num_cubes();
         void initialize();
         void step_physics(double time_step=1.0 / 60.0);
         void output_body_positions_and_rotations(double time_step=1.0 / 60.0);
         void capture_sphere_body_position_and_rotation(AllegroFlare::Vec3D* position=nullptr, AllegroFlare::Vec3D* rotation_euler=nullptr);
         static void build_transform_bullet(ALLEGRO_TRANSFORM* transform=nullptr, AllegroFlare::Placement3D* placement_3d=nullptr);
         void capture_cube_body_position_and_rotation(AllegroFlare::Vec3D* position=nullptr, AllegroFlare::Vec3D* rotation_euler=nullptr, int cube_index=-1);
         void destroy();
      };
   }
}



