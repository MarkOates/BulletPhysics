#pragma once


#include <btBulletDynamicsCommon.h>


namespace BulletPhysics
{
   namespace Examples
   {
      class Basic
      {
      private:
         btDefaultCollisionConfiguration collision_configuration;
         btCollisionDispatcher dispatcher;
         btDbvtBroadphase broadphase;
         btSequentialImpulseConstraintSolver solver;
         btDiscreteDynamicsWorld* dynamics_world;
         btRigidBody* sphere_body;
         btCollisionShape* sphere_shape;
         btRigidBody* ground_body;
         btCollisionShape* ground_shape;
         bool initialized;
         bool destroyed;

      protected:


      public:
         Basic();
         ~Basic();

         void initialize();
         void step_physics(double time_step=1.0 / 60.0);
         void output_body_positions_and_rotations(double time_step=1.0 / 60.0);
         void destroy();
      };
   }
}



