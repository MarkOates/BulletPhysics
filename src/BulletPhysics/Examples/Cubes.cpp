

#include <BulletPhysics/Examples/Cubes.hpp>

#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/Random.hpp>
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
   , cubes({})
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


int Cubes::num_cubes()
{
   return cubes.size();
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

   //dynamics_world->addRigidBody(sphere_body);

   // Create a falling cube
   btVector3 half_extents(0.5, 0.5, 0.5); // Half-dimensions of the cube (1x1x1 cube)
   cube_shape = new btBoxShape(half_extents);
   btDefaultMotionState *cube_motion_state = new btDefaultMotionState(
      btTransform(btQuaternion(0, 0, 0, 1),
      btVector3(0, 12, 0)) // Starting at position
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
   cube_body->setRestitution(0.0);

   dynamics_world->addRigidBody(cube_body);


   AllegroFlare::Random random;


   // Create multiple cubes
   int num_cubes = 12;
   for (int i=0; i<num_cubes; i++)
   {
      // Create a falling cube
      btVector3 half_extents(0.5, 0.5, 0.5); // Half-dimensions of the cube (1x1x1 cube)
      btCollisionShape *cube_shape = new btBoxShape(half_extents);
      btDefaultMotionState *cube_motion_state = new btDefaultMotionState(
         btTransform(btQuaternion(0.125, random.get_random_float(0, 3.125), random.get_random_float(0, 3.125), 1),
         //btVector3(-3 + 1.5*i, 14, 0)) // Starting at position
         btVector3((-2.5 + 1.5*i)/2 + random.get_random_float(-1, 1), 15+0.75 * i, 0)) // Starting at position
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
      btRigidBody *cube_body = new btRigidBody(cube_body_ci);

      // Make the cube body bouncy
      cube_body->setRestitution(0.6);

      dynamics_world->addRigidBody(cube_body);

      cubes.push_back(
         std::pair<btRigidBody*, btCollisionShape*>(cube_body, cube_shape)
         //std::pair<cube_body, cube_shape>
      );
      //type: std::vector<std::pair<btRigidBody*, btCollisionShape*>>
   }


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

void Cubes::build_transform_bullet(ALLEGRO_TRANSFORM* transform, AllegroFlare::Placement3D* placement_3d)
{
   if (!(transform))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Cubes::build_transform_bullet]: error: guard \"transform\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Cubes::build_transform_bullet]: error: guard \"transform\" not met");
   }
   if (!(placement_3d))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Cubes::build_transform_bullet]: error: guard \"placement_3d\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Cubes::build_transform_bullet]: error: guard \"placement_3d\" not met");
   }
   al_identity_transform(transform);

   auto &align = placement_3d->align;
   auto &size = placement_3d->size;
   auto &scale = placement_3d->scale;
   auto &anchor = placement_3d->anchor;
   auto &rotation = placement_3d->rotation;
   auto &position = placement_3d->position;

   // offset for alignment and anchors
   al_translate_transform_3d(transform, -align.x*size.x, -align.y*size.y, -align.z*size.z);
   al_scale_transform_3d(transform, scale.x, scale.y, scale.z);
   al_translate_transform_3d(transform, anchor.x, anchor.y, anchor.z);

   // rotate
   //al_rotate_transform_3d(transform, 0, 0, 1, rotation.z);
   //al_rotate_transform_3d(transform, 0, 1, 0, rotation.y);
   al_rotate_transform_3d(transform, 1, 0, 0, rotation.x);
   al_rotate_transform_3d(transform, 0, 1, 0, rotation.y);
   al_rotate_transform_3d(transform, 0, 0, 1, rotation.z);
   //al_rotate_transform_3d(transform, 0, 0, 1, rotation.z);

   // translate
   al_translate_transform_3d(transform, position.x, position.y, position.z);

   return;
}

void Cubes::capture_cube_body_position_and_rotation(AllegroFlare::Vec3D* position, AllegroFlare::Vec3D* rotation_euler, int cube_index)
{
   if (!(position))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Cubes::capture_cube_body_position_and_rotation]: error: guard \"position\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Cubes::capture_cube_body_position_and_rotation]: error: guard \"position\" not met");
   }
   if (!(rotation_euler))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Cubes::capture_cube_body_position_and_rotation]: error: guard \"rotation_euler\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Cubes::capture_cube_body_position_and_rotation]: error: guard \"rotation_euler\" not met");
   }
   btRigidBody* cube_body = this->cube_body;
   if (cube_index >= 0) cube_body = cubes[cube_index].first;

   // Capture the cube (currently the only object in the scene)
   btTransform bullet_transform;
   cube_body->getMotionState()->getWorldTransform(bullet_transform);
   btVector3 origin = bullet_transform.getOrigin();

   position->x = origin.getX();
   position->y = origin.getY();
   position->z = origin.getZ();

   btQuaternion rotation_quarternion = bullet_transform.getRotation();
   //getEulerZYX (btScalar &yawZ, btScalar &pitchY, btScalar &rollX) const
   btScalar yawZ;
   btScalar pitchY;
   btScalar rollX;

   //btQuaternion rotation_quarternion =
   rotation_quarternion.getEulerZYX(yawZ, pitchY, rollX);
   //getEulerZYX(origin.getX()kbtScalar &yawZ, btScalar &pitchY, btScalar &rollX) const

   //rotation_euler->x = rollX / (3.14159);
   //rotation_euler->y = pitchY / (3.14159);
   //rotation_euler->z = yawZ / (3.14159);
   //const float RAD_TO_TAU = 1.0f / (2.0f * M_PI);
   //rotation_euler->x = pitchY; // * RAD_TO_TAU;
   //rotation_euler->y = yawZ; // * RAD_TO_TAU;
   //rotation_euler->z = rollX; // * RAD_TO_TAU;
   //rotation_euler->x = -rollX; // * RAD_TO_TAU; // x == rollX (or -rollX looks good)
   //rotation_euler->x = yawZ; // * RAD_TO_TAU;
   //rotation_euler->y = pitchY; // * RAD_TO_TAU;
   //rotation_euler->y = yawZ; // * RAD_TO_TAU;

   rotation_euler->y = pitchY; // * RAD_TO_TAU;

   rotation_euler->x = rollX; // * RAD_TO_TAU; // "confirmed"
   rotation_euler->z = yawZ; // * RAD_TO_TAU; // "confirmed"

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

   dynamics_world->removeRigidBody(cube_body);
   delete cube_body->getMotionState();
   delete cube_body;
   delete cube_shape;

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


