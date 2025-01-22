#pragma once


#include <AllegroFlare/Vec3D.hpp>
#include <btBulletDynamicsCommon.h>


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
         btRigidBody* ground_body;
         btCollisionShape* ground_shape;
         bool initialized;
         bool destroyed;

      protected:


      public:
         Cubes();
         ~Cubes();

         void initialize();
         void step_physics(double time_step=1.0 / 60.0);
         void output_body_positions_and_rotations(double time_step=1.0 / 60.0);
         void capture_sphere_body_position_and_rotation(AllegroFlare::Vec3D* position=nullptr, AllegroFlare::Vec3D* rotation_euler=nullptr);
         void capture_cube_body_position_and_rotation(AllegroFlare::Vec3D* position=nullptr, AllegroFlare::Vec3D* rotation_euler=nullptr);
         void destroy();
      };
   }
}



