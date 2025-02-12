

#include <BulletPhysics/DynamicsWorld.hpp>

#include <AllegroFlare/Logger.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace BulletPhysics
{


DynamicsWorld::DynamicsWorld()
   : collision_configuration(nullptr)
   , dispatcher(nullptr)
   , broadphase(nullptr)
   , solver(nullptr)
   , dynamics_world(nullptr)
   , initialized(false)
   , destroyed(false)
{
}


DynamicsWorld::~DynamicsWorld()
{
   if (initialized && !destroyed)
   {
      AllegroFlare::Logger::warn_from(
         "BulletPhysics::DynamicsWorld::~destroy",
         "This instance is being destroyed but the destroy() method has not been called."
      );
   }
   return;
}


btDiscreteDynamicsWorld* DynamicsWorld::get_dynamics_world()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::DynamicsWorld::get_dynamics_world]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::DynamicsWorld::get_dynamics_world]: error: guard \"initialized\" not met");
   }
   if (!((!destroyed)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::DynamicsWorld::get_dynamics_world]: error: guard \"(!destroyed)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::DynamicsWorld::get_dynamics_world]: error: guard \"(!destroyed)\" not met");
   }
   return dynamics_world;
}

void DynamicsWorld::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::DynamicsWorld::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::DynamicsWorld::initialize]: error: guard \"(!initialized)\" not met");
   }
   if (!((!destroyed)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::DynamicsWorld::initialize]: error: guard \"(!destroyed)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::DynamicsWorld::initialize]: error: guard \"(!destroyed)\" not met");
   }
   // Create the required dependencies
   collision_configuration = new btDefaultCollisionConfiguration();
   dispatcher = new btCollisionDispatcher(collision_configuration);
   broadphase = new btDbvtBroadphase();
   solver = new btSequentialImpulseConstraintSolver();

   // Build the dynamics_world
   dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);

   // Set gravity
   dynamics_world->setGravity(btVector3(0, -9.81, 0));

   initialized = true;
   return;
}

void DynamicsWorld::destroy()
{
   if (!((!destroyed)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::DynamicsWorld::destroy]: error: guard \"(!destroyed)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::DynamicsWorld::destroy]: error: guard \"(!destroyed)\" not met");
   }
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::DynamicsWorld::destroy]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::DynamicsWorld::destroy]: error: guard \"initialized\" not met");
   }
   delete dynamics_world;
   delete solver;
   delete broadphase;
   delete dispatcher;
   delete collision_configuration;

   dynamics_world = nullptr;
   solver = nullptr;
   broadphase = nullptr;
   dispatcher = nullptr;
   collision_configuration = nullptr;

   destroyed = true;
   return;
}


} // namespace BulletPhysics


