

#include <BulletPhysics/Examples/Basic.hpp>

#include <AllegroFlare/Logger.hpp>
#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace BulletPhysics
{
namespace Examples
{


Basic::Basic()
   : collision_configuration()
   , dispatcher(&collision_configuration)
   , broadphase()
   , solver()
   , dynamics_world(nullptr)
   , sphere_body(nullptr)
   , sphere_shape(nullptr)
   , ground_body(nullptr)
   , ground_shape(nullptr)
   , initialized(false)
   , destroyed(false)
{
}


Basic::~Basic()
{
   if (initialized && !destroyed)
   {
      AllegroFlare::Logger::warn_from(
         "BulletPhysics::Examples::Basic::~destroy", 
         "This instance is being destroyed but the destroy() method has not been called."
      );
   }
   return;
}


void Basic::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Basic::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Basic::initialize]: error: guard \"(!initialized)\" not met");
   }
   if (!((!destroyed)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Basic::initialize]: error: guard \"(!destroyed)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Basic::initialize]: error: guard \"(!destroyed)\" not met");
   }
   // Create a dynamics world
   //btDefaultCollisionConfiguration collision_configuration;
   //btCollisionDispatcher dispatcher(&collision_configuration);
   //btDbvtBroadphase broadphase;
   //btSequentialImpulseConstraintSolver solver;
   dynamics_world = new btDiscreteDynamicsWorld(&dispatcher, &broadphase, &solver, &collision_configuration);

   // Set gravity
   dynamics_world->setGravity(btVector3(0, -9.81, 0));

   // Create a ground plane
   //btCollisionShape *ground_shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
   ground_shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
   btDefaultMotionState *ground_motion_state = new btDefaultMotionState();
   btRigidBody::btRigidBodyConstructionInfo ground_body_ci(0, ground_motion_state, ground_shape, btVector3(0, 0, 0));
   //btRigidBody *ground_body = new btRigidBody(ground_body_ci);
   ground_body = new btRigidBody(ground_body_ci);
   dynamics_world->addRigidBody(ground_body);

   // Create a falling sphere
   //btCollisionShape *sphere_shape = new btSphereShape(1);
   sphere_shape = new btSphereShape(1);
   btDefaultMotionState *sphere_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
   btScalar sphere_mass = 1;
   btVector3 sphere_inertia;
   sphere_shape->calculateLocalInertia(sphere_mass, sphere_inertia);
   btRigidBody::btRigidBodyConstructionInfo sphere_body_ci(sphere_mass, sphere_motion_state, sphere_shape, sphere_inertia);
   //btRigidBody *sphere_body = new btRigidBody(sphere_body_ci);
   sphere_body = new btRigidBody(sphere_body_ci);
   dynamics_world->addRigidBody(sphere_body);

   initialized = true;
   return;
}

void Basic::_destroy()
{
   if (!((!destroyed)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Basic::_destroy]: error: guard \"(!destroyed)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Basic::_destroy]: error: guard \"(!destroyed)\" not met");
   }
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Basic::_destroy]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Basic::_destroy]: error: guard \"initialized\" not met");
   }
   // Cleanup
   dynamics_world->removeRigidBody(sphere_body);
   //delete sphere_body->getMotionState();
   //delete sphere_body;
   //delete sphere_shape;

   //dynamics_world->removeRigidBody(ground_body);
   //delete ground_body->getMotionState();
   //delete ground_body;
   //delete ground_shape;

   //delete dynamics_world;
   destroyed = true;
   return;
}


} // namespace Examples
} // namespace BulletPhysics


