#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <vector>

// Function to create a box-shaped rigid body
btRigidBody* create_box(btDiscreteDynamicsWorld *world, const btVector3 &half_extents, const btVector3 &position, 
                        btScalar mass, const btVector3 &initial_velocity = btVector3(0, 0, 0))
{
   btCollisionShape *box_shape = new btBoxShape(half_extents);

   btDefaultMotionState *motion_state = new btDefaultMotionState(
      btTransform(btQuaternion(0, 0, 0, 1), position));

   btVector3 inertia(0, 0, 0);
   if (mass > 0)
   {
      box_shape->calculateLocalInertia(mass, inertia);
   }

   btRigidBody::btRigidBodyConstructionInfo rigid_body_info(mass, motion_state, box_shape, inertia);
   btRigidBody *rigid_body = new btRigidBody(rigid_body_info);

   rigid_body->setLinearVelocity(initial_velocity);
   world->addRigidBody(rigid_body);
   return rigid_body;
}

int main()
{
   // Initialize Bullet Physics world
   btBroadphaseInterface *broadphase = new btDbvtBroadphase();
   btDefaultCollisionConfiguration *collision_config = new btDefaultCollisionConfiguration();
   btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collision_config);
   btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
   btDiscreteDynamicsWorld *dynamics_world = new btDiscreteDynamicsWorld(
      dispatcher, broadphase, solver, collision_config);

   dynamics_world->setGravity(btVector3(0, -9.81, 0));

   // Create ground plane
   btCollisionShape *ground_shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
   btDefaultMotionState *ground_motion_state = new btDefaultMotionState(
      btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
   btRigidBody::btRigidBodyConstructionInfo ground_rigid_body_info(0, ground_motion_state, ground_shape, btVector3(0, 0, 0));
   btRigidBody *ground_rigid_body = new btRigidBody(ground_rigid_body_info);

   dynamics_world->addRigidBody(ground_rigid_body);

   // Create dice
   std::vector<btRigidBody *> dice_bodies;
   for (int i = 0; i < 2; ++i)
   {
      btVector3 position(0, 5 + i, i * 2);
      btVector3 velocity(btScalar((rand() % 5) - 2), 5, btScalar((rand() % 5) - 2));
      dice_bodies.push_back(create_box(dynamics_world, btVector3(0.5, 0.5, 0.5), position, 1.0, velocity));
   }

   // Simulate physics
   for (int step = 0; step < 300; ++step)
   {
      dynamics_world->stepSimulation(1.f / 60.f, 10);

      // Output dice positions
      for (size_t i = 0; i < dice_bodies.size(); ++i)
      {
         btTransform transform;
         dice_bodies[i]->getMotionState()->getWorldTransform(transform);
         btVector3 position = transform.getOrigin();
         std::cout << "Dice " << i + 1 << " position: ("
                   << position.getX() << ", " << position.getY() << ", "
                   << position.getZ() << ")\n";
      }
      std::cout << "--------------------\n";
   }

   // Cleanup
   for (auto body : dice_bodies)
   {
      dynamics_world->removeRigidBody(body);
      delete body->getMotionState();
      delete body->getCollisionShape();
      delete body;
   }

   dynamics_world->removeRigidBody(ground_rigid_body);
   delete ground_rigid_body->getMotionState();
   delete ground_shape;
   delete ground_rigid_body;

   delete dynamics_world;
   delete solver;
   delete dispatcher;
   delete collision_config;
   delete broadphase;

   return 0;
}

