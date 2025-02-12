#pragma once


#include <btBulletDynamicsCommon.h>


namespace BulletPhysics
{
   class DynamicsWorld
   {
   private:
      btDefaultCollisionConfiguration* collision_configuration;
      btCollisionDispatcher* dispatcher;
      btDbvtBroadphase* broadphase;
      btSequentialImpulseConstraintSolver* solver;
      btDiscreteDynamicsWorld* dynamics_world;
      bool initialized;
      bool destroyed;

   protected:


   public:
      DynamicsWorld();
      ~DynamicsWorld();

      btDiscreteDynamicsWorld* get_dynamics_world();
      void initialize();
      void destroy();
   };
}



