

#include <BulletPhysics/Examples/Knockdown.hpp>

#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/PlayerInputControllers/Generic.hpp>
#include <AllegroFlare/Random.hpp>
#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace BulletPhysics
{
namespace Examples
{


Knockdown::Knockdown()
   : AllegroFlare::Screens::Gameplay()
   , collision_configuration()
   , dispatcher(&collision_configuration)
   , broadphase()
   , solver()
   , dynamics_world(nullptr)
   , sphere_body(nullptr)
   , sphere_shape(nullptr)
   , cube_shape(nullptr)
   , cubes({})
   , shape_model(nullptr)
   , shapes({})
   , ground_body(nullptr)
   , ground_shape(nullptr)
   , initialized(false)
   , destroyed(false)
{
}


Knockdown::~Knockdown()
{
   if (initialized && !destroyed)
   {
      AllegroFlare::Logger::warn_from(
         "BulletPhysics::Examples::Mesh::~destroy", 
         "This instance is being destroyed but the destroy() method has not been called."
      );
   }
   return;
}


void Knockdown::set_shape_model(AllegroFlare::Model3D* shape_model)
{
   this->shape_model = shape_model;
}


int Knockdown::num_cubes()
{
   return cubes.size();
}

int Knockdown::num_shapes()
{
   return shapes.size();
}

void Knockdown::create_multiple_cubes()
{
   AllegroFlare::Random random;
   random.set_seed(123456+2);

   int num_cubes = 0;
   //btRigidBody* cube_body = nullptr;

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


      // Set initial linear velocity
      btVector3 initial_velocity(0.124, 0, 0); // Move along the x-axis
      cube_body->setLinearVelocity(initial_velocity);

      // Set initial angular velocity
      btVector3 initial_angular_velocity(
         random.get_random_int(-10, 10),
         random.get_random_int(-10, 10),
         random.get_random_int(-10, 10)
      ); // Spin around the y-axis
      cube_body->setAngularVelocity(initial_angular_velocity);

   }

   return;
}

void Knockdown::create_multiple_shapes()
{
   AllegroFlare::Random random;
   random.set_seed(123456+2);

   // Create multiple shapes
   //int num_shapes = 11;
   int num_shapes = 8;
   float shape_spacing = 2.25;
   for (int i=0; i<num_shapes; i++)
   {
      //shape_model
      btRigidBody* shape_body = nullptr;
      btCollisionShape* shape_shape = nullptr;

      std::tie(shape_body, shape_shape) = create_dynamic_mesh_shape(
         shape_model,
         0.0 - num_shapes * 0.5 * shape_spacing + shape_spacing * i,
         10,
         0
      );
    
      // Set initial angular velocity
      btVector3 initial_angular_velocity(
         random.get_random_int(-2, 2),
         random.get_random_int(-2, 2),
         random.get_random_int(-2, 2)
      ); // Spin around the y-axis
      shape_body->setAngularVelocity(initial_angular_velocity);

      //create_dynamic_mesh_shape
      
      // Make the cube body bouncy

      dynamics_world->addRigidBody(shape_body);

      shapes.push_back(
         std::pair<btRigidBody*, btCollisionShape*>(shape_body, shape_shape)
         //std::pair<cube_body, cube_shape>
      );
      //type: std::vector<std::pair<btRigidBody*, btCollisionShape*>>


      //// Set initial linear velocity
      //btVector3 initial_velocity(0.124, 0, 0); // Move along the x-axis
      //cube_body->setLinearVelocity(initial_velocity);

      //// Set initial angular velocity
      //btVector3 initial_angular_velocity(
         //random.get_random_int(-10, 10),
         //random.get_random_int(-10, 10),
         //random.get_random_int(-10, 10)
      //); // Spin around the y-axis
      //cube_body->setAngularVelocity(initial_angular_velocity);

   }


   return;
}

void Knockdown::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::initialize]: error: guard \"(!initialized)\" not met");
   }
   if (!((!destroyed)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::initialize]: error: guard \"(!destroyed)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::initialize]: error: guard \"(!destroyed)\" not met");
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



   create_multiple_cubes();



   create_multiple_shapes();



   // Setup a player_input_controller
   auto generic_player_input_controller = new AllegroFlare::PlayerInputControllers::Generic();
   generic_player_input_controller->set_on_key_pressed([this](int key_code){
      switch (key_code)
      {
         case ALLEGRO_KEY_P: {
            // TODO: Replace this manual suspending with an "call_on_paused_callback_func()"
            {
               if (get_gameplay_suspended()) resume_suspended_gameplay();
               else suspend_gameplay();
            }
         } break;
      }
   });
   set_player_input_controller(generic_player_input_controller);


   initialized = true;
   return;
}

btConvexHullShape* Knockdown::create_convex_shape(AllegroFlare::Model3D* model)
{
   if (!(model))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::create_convex_shape]: error: guard \"model\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::create_convex_shape]: error: guard \"model\" not met");
   }
   // TODO: Confirm model is triangulated, and points are TRIANGLE_LINE_LIST
   btConvexHullShape *convex_shape = new btConvexHullShape();

   for (auto &vertex : model->vertices)
   {
       btVector3 bt_vertex(vertex.x, vertex.y, vertex.z);
       convex_shape->addPoint(bt_vertex, false); //  avoids automatic recalculation of the AABB
   }
   convex_shape->recalcLocalAabb(); // Recalculate the bounding box after all points are added
   return convex_shape;
}

std::pair<btRigidBody*, btCollisionShape*> Knockdown::create_dynamic_mesh_shape(AllegroFlare::Model3D* model, float x, float y, float z)
{
   if (!(model))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::create_dynamic_mesh_shape]: error: guard \"model\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::create_dynamic_mesh_shape]: error: guard \"model\" not met");
   }
   btConvexHullShape *convex_shape = create_convex_shape(model);

   btScalar mass = 1;  // Adjust mass as required
   btVector3 inertia(0, 0, 0);
   convex_shape->calculateLocalInertia(mass, inertia);

   btDefaultMotionState *motion_state = new btDefaultMotionState(
       //btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0))
       btTransform(btQuaternion(0, 0, 0, 1), btVector3(x, y, z))
   );

   btRigidBody::btRigidBodyConstructionInfo body_ci(
       mass,
       motion_state,
       convex_shape,
       inertia
   );

   btRigidBody *dynamic_body = new btRigidBody(body_ci);

   dynamic_body->setRestitution(0.6);

   // NOTE: The object will need to be added to the dynamics world after being returned from this method:
   //dynamics_world->addRigidBody(dynamic_body);

   return std::make_pair(dynamic_body, convex_shape);
   //return std::make_pair<btRigidBody*, btCollisionShape*>(dynamic_body, convex_shape);
}

void Knockdown::step_physics(double time_step)
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

void Knockdown::output_body_positions_and_rotations(double time_step)
{
   // Output the sphere (currently the only object in the scene)
   btTransform bullet_transform;
   sphere_body->getMotionState()->getWorldTransform(bullet_transform);
   std::cout << "Sphere height at step: " << bullet_transform.getOrigin().getY() << std::endl;
   return;
}

void Knockdown::capture_sphere_body_position_and_rotation(AllegroFlare::Vec3D* position, AllegroFlare::Vec3D* rotation_euler)
{
   if (!(position))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_sphere_body_position_and_rotation]: error: guard \"position\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_sphere_body_position_and_rotation]: error: guard \"position\" not met");
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

void Knockdown::build_transform_bullet(ALLEGRO_TRANSFORM* transform, AllegroFlare::Placement3D* placement_3d)
{
   if (!(transform))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::build_transform_bullet]: error: guard \"transform\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::build_transform_bullet]: error: guard \"transform\" not met");
   }
   if (!(placement_3d))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::build_transform_bullet]: error: guard \"placement_3d\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::build_transform_bullet]: error: guard \"placement_3d\" not met");
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

void Knockdown::capture_cube_body_position_and_rotation(AllegroFlare::Vec3D* position, AllegroFlare::Vec3D* rotation_euler, int cube_index)
{
   if (!(position))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_cube_body_position_and_rotation]: error: guard \"position\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_cube_body_position_and_rotation]: error: guard \"position\" not met");
   }
   if (!(rotation_euler))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_cube_body_position_and_rotation]: error: guard \"rotation_euler\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_cube_body_position_and_rotation]: error: guard \"rotation_euler\" not met");
   }
   if (!((cube_index >= 0)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_cube_body_position_and_rotation]: error: guard \"(cube_index >= 0)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_cube_body_position_and_rotation]: error: guard \"(cube_index >= 0)\" not met");
   }
   if (!((cube_index < cubes.size())))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_cube_body_position_and_rotation]: error: guard \"(cube_index < cubes.size())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_cube_body_position_and_rotation]: error: guard \"(cube_index < cubes.size())\" not met");
   }
   btRigidBody* cube_body = cubes[cube_index].first;
   //if (cube_index >= 0) cube_body = cubes[cube_index].first;
   //if (cube_index >= 0) cube_body = cubes[cube_index].first;

   btTransform bullet_transform;
   cube_body->getMotionState()->getWorldTransform(bullet_transform);
   btVector3 origin = bullet_transform.getOrigin();

   position->x = origin.getX();
   position->y = origin.getY();
   position->z = origin.getZ();

   btQuaternion rotation_quarternion = bullet_transform.getRotation();

   btScalar yawZ;
   btScalar pitchY;
   btScalar rollX;

   rotation_quarternion.getEulerZYX(yawZ, pitchY, rollX);
   rotation_euler->y = pitchY;
   rotation_euler->x = rollX;
   rotation_euler->z = yawZ;

   return;
}

void Knockdown::capture_shape_body_position_and_rotation(AllegroFlare::Vec3D* position, AllegroFlare::Vec3D* rotation_euler, int shape_index)
{
   if (!(position))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_shape_body_position_and_rotation]: error: guard \"position\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_shape_body_position_and_rotation]: error: guard \"position\" not met");
   }
   if (!(rotation_euler))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_shape_body_position_and_rotation]: error: guard \"rotation_euler\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_shape_body_position_and_rotation]: error: guard \"rotation_euler\" not met");
   }
   if (!((shape_index >= 0)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_shape_body_position_and_rotation]: error: guard \"(shape_index >= 0)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_shape_body_position_and_rotation]: error: guard \"(shape_index >= 0)\" not met");
   }
   if (!((shape_index < shapes.size())))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_shape_body_position_and_rotation]: error: guard \"(shape_index < shapes.size())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_shape_body_position_and_rotation]: error: guard \"(shape_index < shapes.size())\" not met");
   }
   btRigidBody* shape_body = shapes[shape_index].first;

   capture_rigid_body_position_and_rotation(position, rotation_euler, shape_body);

   return;
}

void Knockdown::capture_rigid_body_position_and_rotation(AllegroFlare::Vec3D* position, AllegroFlare::Vec3D* rotation_euler, btRigidBody* rigid_body)
{
   if (!(position))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_rigid_body_position_and_rotation]: error: guard \"position\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_rigid_body_position_and_rotation]: error: guard \"position\" not met");
   }
   if (!(rotation_euler))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_rigid_body_position_and_rotation]: error: guard \"rotation_euler\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_rigid_body_position_and_rotation]: error: guard \"rotation_euler\" not met");
   }
   if (!(rigid_body))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_rigid_body_position_and_rotation]: error: guard \"rigid_body\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_rigid_body_position_and_rotation]: error: guard \"rigid_body\" not met");
   }
   btTransform bullet_transform;
   rigid_body->getMotionState()->getWorldTransform(bullet_transform);
   btVector3 origin = bullet_transform.getOrigin();

   position->x = origin.getX();
   position->y = origin.getY();
   position->z = origin.getZ();

   btQuaternion rotation_quarternion = bullet_transform.getRotation();

   btScalar yawZ;
   btScalar pitchY;
   btScalar rollX;

   rotation_quarternion.getEulerZYX(yawZ, pitchY, rollX);
   rotation_euler->y = pitchY;
   rotation_euler->x = rollX;
   rotation_euler->z = yawZ;

   return;
}

void Knockdown::destroy()
{
   if (!((!destroyed)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::destroy]: error: guard \"(!destroyed)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::destroy]: error: guard \"(!destroyed)\" not met");
   }
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::destroy]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::destroy]: error: guard \"initialized\" not met");
   }
   // Cleanup
   dynamics_world->removeRigidBody(sphere_body);
   delete sphere_body->getMotionState();
   delete sphere_body;
   delete sphere_shape;

   /*
   dynamics_world->removeRigidBody(cube_body);
   delete cube_body->getMotionState();
   delete cube_body;
   */
   delete cube_shape;

   dynamics_world->removeRigidBody(ground_body);
   delete ground_body->getMotionState();
   delete ground_body;
   delete ground_shape;

   // TODO: Delete properties of cubes

   delete dynamics_world;
   destroyed = true;
   return;
}

void Knockdown::primary_update_func(double time_now, double time_step)
{
   if (get_gameplay_suspended()) return;

   step_physics(time_step);
   // Simulate physics
   //for (int i = 0; i < 150; i++)
   //{
      //dynamics_world.stepSimulation(1.0f / 60.0f, 10);
      //dynamics_world->stepSimulation(time_step, 10); // TODO: Not sure what 10 is, believe it's "max substeps", but
                                                     // not sure how this applies
      //btTransform trans;
      //sphere_body->getMotionState()->getWorldTransform(trans);
      //std::cout << "Sphere height at step " << i << ": " << trans.getOrigin().getY() << std::endl;
   //}
   return;
}


} // namespace Examples
} // namespace BulletPhysics


