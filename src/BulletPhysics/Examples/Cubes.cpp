

#include <BulletPhysics/Examples/Cubes.hpp>

#include <AllegroFlare/Logger.hpp>
#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace BulletPhysics
{
namespace Examples
{


Cubes::Cubes()
   : collision_configuration()
   , dispatcher(&collision_configuration)
   , broadphase()
   , solver()
   , dynamics_world(nullptr)
   , sphere_body(nullptr)
   , sphere_shape(nullptr)
   , cube_body(nullptr)
   , cube_shape(nullptr)
   , ground_body(nullptr)
   , ground_shape(nullptr)
   , initialized(false)
   , destroyed(false)
{
}


Cubes::~Cubes()
{
   if (initialized && !destroyed)
   {
      AllegroFlare::Logger::warn_from(
         "BulletPhysics::Examples::Cubes::~destroy", 
         "This instance is being destroyed but the destroy() method has not been called."
      );
   }
   return;
}


void Cubes::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Cubes::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Cubes::initialize]: error: guard \"(!initialized)\" not met");
   }
   if (!((!destroyed)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Cubes::initialize]: error: guard \"(!destroyed)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Cubes::initialize]: error: guard \"(!destroyed)\" not met");
   }
   // Create a dynamics world
   //btDefaultCollisionConfiguration collision_configuration;
   //btCollisionDispatcher dispatcher(&collision_configuration);
   //btDbvtBroadphase broadphase;
   //btSequentialImpulseConstraintSolver solver;
   // NOTE: I'm not sure what of these injected objects are disposable after construction and/or which need to be
   // kept alive for dynamics_world
   dynamics_world = new btDiscreteDynamicsWorld(&dispatcher, &broadphase, &solver, &collision_configuration);

   // Set gravity
   dynamics_world->setGravity(btVector3(0, -9.81, 0));

   // Create a ground plane
   ground_shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
   btDefaultMotionState *ground_motion_state = new btDefaultMotionState();
   btRigidBody::btRigidBodyConstructionInfo ground_body_ci(0, ground_motion_state, ground_shape, btVector3(0, 0, 0));
   ground_body = new btRigidBody(ground_body_ci);
   dynamics_world->addRigidBody(ground_body);

   // Make the ground floor body bouncy
   ground_body->setRestitution(0.8);

   // Create a falling sphere
   sphere_shape = new btSphereShape(1);
   btDefaultMotionState *sphere_motion_state = new btDefaultMotionState(
      btTransform(btQuaternion(0, 0, 0, 1),
      btVector3(0, 10, 0))
   );
   btScalar sphere_mass = 1;
   btVector3 sphere_inertia;
   sphere_shape->calculateLocalInertia(sphere_mass, sphere_inertia);
   btRigidBody::btRigidBodyConstructionInfo sphere_body_ci(
      sphere_mass,
      sphere_motion_state,
      sphere_shape,
      sphere_inertia
   );
   sphere_body = new btRigidBody(sphere_body_ci);

   // Make the sphere body bouncy
   sphere_body->setRestitution(0.8);

   dynamics_world->addRigidBody(sphere_body);

   // Create a falling cube
   btVector3 half_extents(1, 1, 1); // Half-dimensions of the cube (1x1x1 cube)
   cube_shape = new btBoxShape(half_extents);
   btDefaultMotionState *cube_motion_state = new btDefaultMotionState(
      btTransform(btQuaternion(0, 0, 0, 1),
      btVector3(2.5, 12, 0)) // Starting at position
   );
   btScalar cube_mass = 1;
   btVector3 cube_inertia;
   cube_shape->calculateLocalInertia(cube_mass, cube_inertia); // Calculate the cube's inertia tensor
   btRigidBody::btRigidBodyConstructionInfo cube_body_ci(
      cube_mass,
      cube_motion_state,
      cube_shape,
      cube_inertia
   );
   cube_body = new btRigidBody(cube_body_ci);

   // Make the cube body bouncy
   cube_body->setRestitution(0.8);

   dynamics_world->addRigidBody(cube_body);


   initialized = true;
   return;
}

void Cubes::step_physics(double time_step)
{
   // Simulate physics
   //for (int i = 0; i < 150; i++)
   //{
      //dynamics_world.stepSimulation(1.0f / 60.0f, 10);
      dynamics_world->stepSimulation(time_step, 10); // TODO: Not sure what 10 is, believe it's "max substeps", but
                                                     // not sure how this applies
      btTransform trans;
      sphere_body->getMotionState()->getWorldTransform(trans);
      //std::cout << "Sphere height at step " << i << ": " << trans.getOrigin().getY() << std::endl;
   //}
   return;
}

void Cubes::output_body_positions_and_rotations(double time_step)
{
   // Output the sphere (currently the only object in the scene)
   btTransform bullet_transform;
   sphere_body->getMotionState()->getWorldTransform(bullet_transform);
   std::cout << "Sphere height at step: " << bullet_transform.getOrigin().getY() << std::endl;
   return;
}

void Cubes::capture_sphere_body_position_and_rotation(AllegroFlare::Vec3D* position, AllegroFlare::Vec3D* rotation_euler)
{
   if (!(position))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Cubes::capture_sphere_body_position_and_rotation]: error: guard \"position\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Cubes::capture_sphere_body_position_and_rotation]: error: guard \"position\" not met");
   }
   // Capture the sphere (currently the only object in the scene)
   btTransform bullet_transform;
   sphere_body->getMotionState()->getWorldTransform(bullet_transform);
   btVector3 origin = bullet_transform.getOrigin();

   position->x = origin.getX();
   position->y = origin.getY();
   position->z = origin.getZ();

   //btQuaternion rotation_quarternion = bullet_transform.getRotation();

   //rotation_euler->x = origin.getX();
   //rotation_euler->y = origin.getY();
   //rotation_euler->z = origin.getZ();

   return;
}

void Cubes::capture_cube_body_position_and_rotation(AllegroFlare::Vec3D* position, AllegroFlare::Vec3D* rotation_euler)
{
   if (!(position))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Cubes::capture_cube_body_position_and_rotation]: error: guard \"position\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Cubes::capture_cube_body_position_and_rotation]: error: guard \"position\" not met");
   }
   // Capture the cube (currently the only object in the scene)
   btTransform bullet_transform;
   cube_body->getMotionState()->getWorldTransform(bullet_transform);
   btVector3 origin = bullet_transform.getOrigin();

   position->x = origin.getX();
   position->y = origin.getY();
   position->z = origin.getZ();

   //btQuaternion rotation_quarternion = bullet_transform.getRotation();

   //rotation_euler->x = origin.getX();
   //rotation_euler->y = origin.getY();
   //rotation_euler->z = origin.getZ();

   return;
}

void Cubes::destroy()
{
   if (!((!destroyed)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Cubes::destroy]: error: guard \"(!destroyed)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Cubes::destroy]: error: guard \"(!destroyed)\" not met");
   }
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Cubes::destroy]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Cubes::destroy]: error: guard \"initialized\" not met");
   }
   // Cleanup
   dynamics_world->removeRigidBody(sphere_body);
   delete sphere_body->getMotionState();
   delete sphere_body;
   delete sphere_shape;

   //dynamics_world->removeRigidBody(cube_body);
   //delete cube_body->getMotionState();
   //delete cube_body;
   //delete cube_shape;

   dynamics_world->removeRigidBody(ground_body);
   delete ground_body->getMotionState();
   delete ground_body;
   delete ground_shape;

   delete dynamics_world;
   destroyed = true;
   return;
}


} // namespace Examples
} // namespace BulletPhysics


