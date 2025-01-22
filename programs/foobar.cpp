 
#include <btBulletDynamicsCommon.h>
#include <iostream>

#include <allegro5/allegro.h>

int main(int argc, char **argv)
{
//int main()
//{
   // Create a dynamics world
   btDefaultCollisionConfiguration collision_configuration;
   btCollisionDispatcher dispatcher(&collision_configuration);
   btDbvtBroadphase broadphase;
   btSequentialImpulseConstraintSolver solver;
   btDiscreteDynamicsWorld dynamics_world(&dispatcher, &broadphase, &solver, &collision_configuration);

   // Set gravity
   dynamics_world.setGravity(btVector3(0, -9.81, 0));

   // Create a ground plane
   btCollisionShape *ground_shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
   btDefaultMotionState *ground_motion_state = new btDefaultMotionState();
   btRigidBody::btRigidBodyConstructionInfo ground_body_ci(0, ground_motion_state, ground_shape, btVector3(0, 0, 0));
   btRigidBody *ground_body = new btRigidBody(ground_body_ci);
   dynamics_world.addRigidBody(ground_body);

   // Create a falling sphere
   btCollisionShape *sphere_shape = new btSphereShape(1);
   btDefaultMotionState *sphere_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
   btScalar sphere_mass = 1;
   btVector3 sphere_inertia;
   sphere_shape->calculateLocalInertia(sphere_mass, sphere_inertia);
   btRigidBody::btRigidBodyConstructionInfo sphere_body_ci(sphere_mass, sphere_motion_state, sphere_shape, sphere_inertia);
   btRigidBody *sphere_body = new btRigidBody(sphere_body_ci);
   dynamics_world.addRigidBody(sphere_body);

   // Simulate physics
   for (int i = 0; i < 150; i++)
   {
      dynamics_world.stepSimulation(1.0f / 60.0f, 10);

      btTransform trans;
      sphere_body->getMotionState()->getWorldTransform(trans);
      std::cout << "Sphere height at step " << i << ": " << trans.getOrigin().getY() << std::endl;
   }

   // Cleanup
   dynamics_world.removeRigidBody(sphere_body);
   delete sphere_body->getMotionState();
   delete sphere_body;
   delete sphere_shape;

   dynamics_world.removeRigidBody(ground_body);
   delete ground_body->getMotionState();
   delete ground_body;
   delete ground_shape;

   return 0;
}

