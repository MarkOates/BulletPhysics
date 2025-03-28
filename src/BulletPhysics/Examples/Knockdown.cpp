

#include <BulletPhysics/Examples/Knockdown.hpp>

#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/Camera2D.hpp>
#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/FontBin.hpp>
#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/PlayerInputControllers/Generic.hpp>
#include <AllegroFlare/Random.hpp>
#include <AllegroFlare/Tiled/TMJDataLoader.hpp>
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
   , data_folder_path("[unset-data_folder_path]")
   , player_inventory_items()
   , player_num_lives(8)
   , dynamics_world(nullptr)
   , dynamics_world_object(nullptr)
   , sphere_body(nullptr)
   , sphere_diameter(1.25f)
   , dip_to_black_opacity(0.0f)
   , sphere_initial_position(btVector3(0, sphere_diameter * 2, 6))
   , sphere_shape(nullptr)
   , cube_shape(nullptr)
   , cubes({})
   , cube_initial_heights({})
   , shape_model(nullptr)
   , shapes({})
   , ground_body(nullptr)
   , ground_shape(nullptr)
   , player_has_thrown_ball(false)
   , initialized(false)
   , game_world_is_setup_and_requires_destruction(false)
   , destroyed(false)
   , gameplay_meta_state({})
   , camera3d({})
   , hud_camera({})
   , model_bin({})
   , bitmap_bin({})
   , font_bin({})
{
}


Knockdown::~Knockdown()
{
   if (initialized && !destroyed)
   {
      AllegroFlare::Logger::warn_from(
         "BulletPhysics::Examples::Knockdown::~destroy",
         "This instance is being destroyed but the destroy() method has not been called."
      );
   }
   if (game_world_is_setup_and_requires_destruction)
   {
      AllegroFlare::Logger::warn_from(
         "BulletPhysics::Examples::Knockdown::~destroy",
         "This instance is indicating that \"game_world_is_setup_and_requires_destruction == true\". Ensure that "
            "the necessary things are destroyed. If not, it will likely result in a crash at program close."
      );
   }
   return;
}


void Knockdown::set_data_folder_path(std::string data_folder_path)
{
   this->data_folder_path = data_folder_path;
}


void Knockdown::set_player_inventory_items(std::vector<std::string> player_inventory_items)
{
   this->player_inventory_items = player_inventory_items;
}


void Knockdown::set_player_num_lives(int player_num_lives)
{
   this->player_num_lives = player_num_lives;
}


void Knockdown::set_shape_model(AllegroFlare::Model3D* shape_model)
{
   this->shape_model = shape_model;
}


std::string Knockdown::get_data_folder_path() const
{
   return data_folder_path;
}


std::vector<std::string> Knockdown::get_player_inventory_items() const
{
   return player_inventory_items;
}


int Knockdown::get_player_num_lives() const
{
   return player_num_lives;
}


int Knockdown::num_cubes()
{
   return cubes.size();
}

int Knockdown::num_shapes()
{
   return shapes.size();
}

bool Knockdown::load_level_by_identifier(std::string possible_type)
{
   //clear(); // TODO: Guard against clearing after already clearing
   reset();
   return true;
}

void Knockdown::reset()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::reset]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::reset]: error: guard \"initialized\" not met");
   }
   if (game_world_is_setup_and_requires_destruction) clear();

   // Create a dynamics world
   dynamics_world_object = new BulletPhysics::DynamicsWorld();
   dynamics_world_object->initialize();
   dynamics_world = dynamics_world_object->get_dynamics_world();


   // Create a ground plane
   ground_shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
   btDefaultMotionState *ground_motion_state = new btDefaultMotionState();
   btRigidBody::btRigidBodyConstructionInfo ground_body_ci(0, ground_motion_state, ground_shape, btVector3(0, 0, 0));
   ground_body = new btRigidBody(ground_body_ci);
   dynamics_world->addRigidBody(ground_body);

   // Make the ground floor body bouncy
   ground_body->setRestitution(0.8);


   // Create the player's sphere
   double diameter = sphere_diameter;
   sphere_shape = new btSphereShape(diameter); // Sphere has a size of radius of 1 (diameter of 2)
   btDefaultMotionState *sphere_motion_state = new btDefaultMotionState(
      btTransform(
         btQuaternion(0, 0, 0, 1),
         //btVector3(5, diameter * 2, 0)
         sphere_initial_position
      )
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


   // Create the game world and start the simulation
   player_has_thrown_ball = false;
   create_stacked_cubes();
   if (get_gameplay_suspended()) resume_suspended_gameplay();
   set_camera_to_start_position();
   //if (get_gameplay_suspended()) resume_suspended_gameplay();
   //gameplay_meta_state.start_opening_sequence();
   gameplay_meta_state.start_opening_sequence();
   //set_state(STATE_OPENING_SEQUENCE);
   //set_state(STATE_WAITING_FOR_PLAYER_TO_THROW_BALL);

   game_world_is_setup_and_requires_destruction = true;

   return;
}

void Knockdown::clear()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::clear]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::clear]: error: guard \"initialized\" not met");
   }
   if (!game_world_is_setup_and_requires_destruction) return;


   // Delete the memory elements of the cubes
   for (auto &cube : cubes)
   {
      dynamics_world->removeRigidBody(cube.first);
      delete cube.first->getMotionState(); // delete btDefaultMotionState
      delete cube.first; // delete btRigidBody
      delete cube.second; // delete btCollisionShape
   }

   // Clear the vectors
   cubes.clear();
   cube_initial_heights.clear();

   // Clear the ground plane
   dynamics_world->removeRigidBody(ground_body);
   delete ground_body->getMotionState(); // Delete the motion state
   delete ground_body; // Delete the rigid body
   delete ground_shape; // Delete the collision shape
   ground_body = nullptr;
   ground_shape = nullptr;

   // Clear the player_sphere
   dynamics_world->removeRigidBody(sphere_body); // Remove from the physics world
   delete sphere_body->getMotionState(); // Delete the motion state
   delete sphere_body; // Delete the rigid body
   delete sphere_shape; // Delete the collision shape
   sphere_body = nullptr;
   sphere_shape = nullptr;

   // For debugging purposes, output the number of bodies remaining in the dynamics_world
   int num_bodies = dynamics_world->getNumCollisionObjects();
   AllegroFlare::Logger::info_from(THIS_CLASS_AND_METHOD_NAME,
      "Number of remaining rigid bodies in the dynamic_world: " + std::to_string(num_bodies)
   );


   // Delete the dynamics_world object
   // Clear our local class's dynamics_world
   dynamics_world = nullptr;
   // Destroy the dynamics_world
   dynamics_world_object->destroy();
   delete dynamics_world_object;


   // Clear the local bins (there may have been models used for rendering or loaded for collision shapes)
   model_bin.clear();
   bitmap_bin.clear();
   font_bin.clear();


   game_world_is_setup_and_requires_destruction = false;

   return;
}

void Knockdown::create_multiple_cubes()
{
   AllegroFlare::Random random;
   random.set_seed(123456+2);

   int num_cubes = 20;
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

void Knockdown::create_stacked_cubes()
{
   //AllegroFlare::Random random;
   //random.set_seed(123456+2);

   //int num_cubes = 20;
   //btRigidBody* cube_body = nullptr;

   int num_rows = 4;
   int num_columns= 3;


   for (int y=0; y<num_columns; y++)
   {
      for (int x=0; x<num_rows; x++)
      {
         float pos_x = x - (num_rows * 0.5) + 0.5;
         // Define a cube height
         float cube_height = y + 1.5;

         // Create a cube
         btVector3 half_extents(0.5, 0.5, 0.5); // Half-dimensions of the cube (1x1x1 cube)
         btCollisionShape *cube_shape = new btBoxShape(half_extents);
         btDefaultMotionState *cube_motion_state = new btDefaultMotionState(
            btTransform(btQuaternion(0.0, 0.0, 0.0, 1),
            //btTransform(btQuaternion(0.125, random.get_random_float(0, 3.125), random.get_random_float(0, 3.125), 1),
            //btVector3(-3 + 1.5*i, 14, 0)) // Starting at position
            btVector3(pos_x, cube_height, 0)) // Starting at position
            //btVector3((-2.5 + 1.5*i)/2 + random.get_random_float(-1, 1), 15+0.75 * i, 0)) // Starting at position
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
         cube_initial_heights.push_back(cube_height);
         //type: std::vector<std::pair<btRigidBody*, btCollisionShape*>>


         // Set initial linear velocity
         btVector3 initial_velocity(0, 0, 0); // Move along the x-axis
         //btVector3 initial_velocity(0.124, 0, 0); // Move along the x-axis
         cube_body->setLinearVelocity(initial_velocity);

         // Set initial angular velocity
         btVector3 initial_angular_velocity(
            0, 0, 0
            //random.get_random_int(-10, 10),
            //random.get_random_int(-10, 10),
            //random.get_random_int(-10, 10)
         ); // Spin around the y-axis
         cube_body->setAngularVelocity(initial_angular_velocity);

      }
   }

   return;
}

void Knockdown::create_multiple_shapes()
{
   AllegroFlare::Random random;
   random.set_seed(123456+2);

   // Create multiple shapes
   //int num_shapes = 11;
   int num_shapes = 0;
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

void Knockdown::create_shapes_from_tmj_file()
{
   std::string filename = data_folder_path + "maps/" + "stack-01.tmj";
   AllegroFlare::Tiled::TMJDataLoader tmj_data_loader(filename);
   tmj_data_loader.load();

   tmj_data_loader.for_each_object([](AllegroFlare::Tiled::TMJObject* object, void* user_data){
      // HERE
      // TODO: Parse the object into a cube
   });

   return;
}

BulletPhysics::Examples::Knockdown* Knockdown::create()
{
   BulletPhysics::Examples::Knockdown *result = new BulletPhysics::Examples::Knockdown();
   // TODO: Put together required pieces
   return result;
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
   // TODO: Separate the "initializing" from the creation of the elements in the world and their destruction.

   //reset();

   //// Create a dynamics world
   //dynamics_world_object = new BulletPhysics::DynamicsWorld();
   //dynamics_world_object->initialize();
   //dynamics_world = dynamics_world_object->get_dynamics_world();


   //// Create a ground plane
   //ground_shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
   //btDefaultMotionState *ground_motion_state = new btDefaultMotionState();
   //btRigidBody::btRigidBodyConstructionInfo ground_body_ci(0, ground_motion_state, ground_shape, btVector3(0, 0, 0));
   //ground_body = new btRigidBody(ground_body_ci);
   //dynamics_world->addRigidBody(ground_body);

   //// Make the ground floor body bouncy
   //ground_body->setRestitution(0.8);


   //// Create the player's sphere
   //double diameter = sphere_diameter;
   //sphere_shape = new btSphereShape(diameter); // Sphere has a size of radius of 1 (diameter of 2)
   //btDefaultMotionState *sphere_motion_state = new btDefaultMotionState(
      //btTransform(
         //btQuaternion(0, 0, 0, 1),
         ////btVector3(5, diameter * 2, 0)
         //sphere_initial_position
      //)
   //);
   //btScalar sphere_mass = 1;
   //btVector3 sphere_inertia;
   //sphere_shape->calculateLocalInertia(sphere_mass, sphere_inertia);
   //btRigidBody::btRigidBodyConstructionInfo sphere_body_ci(
      //sphere_mass,
      //sphere_motion_state,
      //sphere_shape,
      //sphere_inertia
   //);
   //sphere_body = new btRigidBody(sphere_body_ci);

   //// Make the sphere body bouncy
   //sphere_body->setRestitution(0.8);

   //dynamics_world->addRigidBody(sphere_body);



   // Add some cubes to the scene
   //create_multiple_cubes();
   //create_stacked_cubes();



   // Add some shapes to the scene
   //create_multiple_shapes();



   // Load objects from a TMJ file
   //create_shapes_from_tmj_file();




   // Setup a player_input_controller
   auto generic_player_input_controller = new AllegroFlare::PlayerInputControllers::Generic();
   generic_player_input_controller->set_on_key_pressed([this](int key_code){
      if (!gameplay_meta_state.is_player_input_active()) return;

      if (this->waiting_for_player_input_to_continue())
      {
         advance_from_waiting_for_player_input_to_continue();
         return;
      }

      switch (key_code)
      {
         //case ALLEGRO_KEY_P: {
            // TODO: Replace this manual suspending with an "call_on_paused_callback_func()"
            //call_on_paused_callback_func();
            //{
               //if (get_gameplay_suspended()) resume_suspended_gameplay();
               //else suspend_gameplay();
            //}
         //} break;

         //case ALLEGRO_KEY_R: {
            //clear();
            //reset();
         //} break;

         case ALLEGRO_KEY_M: {
            save_to_manual_save(); // from Screens::Gameplay
         }; break;

         case ALLEGRO_KEY_Q: {
            save_to_quicksave_save(); // from Screens::Gameplay
         }; break;

         case ALLEGRO_KEY_A: {
            save_to_autosave_save(); // from Screens::Gameplay
         }; break;

         case ALLEGRO_KEY_SPACE: {
            //if (is_state(STATE_WAITING_FOR_PLAYER_TO_THROW_BALL))
            if (!player_has_thrown_ball)
            {
               AllegroFlare::Random random;
               random.set_seed(al_get_time() * 1000);
               //btVector3 initial_position(1, sphere_diameter * 2, 4);
               float x_vel = random.get_random_float(-5, 5);
               btVector3 velocity(x_vel, 0, -20);
               launch_ball(
                  &sphere_initial_position,
                  &velocity
               );
            }
         } break;
      }
   });
   set_player_input_controller(generic_player_input_controller);



   // Setup the gameplay_meta_state to handle when the level is finished
   gameplay_meta_state.set_on_closed_out_func([this](){
      //clear();
      reset();
   });



   //initialized = true;



   // Initialize the rendering
   initialize_render();

   //set_state(STATE_WAITING_TO_START);
   gameplay_meta_state.set_state_to_loaded_and_waiting_to_start();
   //GAMEPLAY_META_STATE_WAITING_TO_START

   initialized = true;



   // Start the game (this now happens at load_level_by_identifier)
   //clear();
   reset(); // TODO: See if this can be moved to a kind of "start()" method and not included in "initialize()"

   // Start the game
   //set_state(STATE_WAITING_FOR_PLAYER_TO_THROW_BALL);


   return;
}

void Knockdown::launch_ball(btVector3* position_, btVector3* velocity_)
{
   if (!(position_))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::launch_ball]: error: guard \"position_\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::launch_ball]: error: guard \"position_\" not met");
   }
   if (!(velocity_))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::launch_ball]: error: guard \"velocity_\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::launch_ball]: error: guard \"velocity_\" not met");
   }
   const btVector3 &position = *position_;
   const btVector3 &velocity = *velocity_;
   //void MyClass::launch_ball(const btVector3 &position, const btVector3 &velocity)
   //{
   // Wake up the object if it's sleeping
   sphere_body->setActivationState(ACTIVE_TAG);

   // Reset the sphere's position
   sphere_body->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), position));
   sphere_body->getMotionState()->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), position));

   // Clear any previous velocity
   sphere_body->setLinearVelocity(btVector3(0, 0, 0));
   sphere_body->setAngularVelocity(btVector3(0, 0, 0));

   // Apply an impulse to "throw" the ball
   sphere_body->applyCentralImpulse(velocity);


   player_has_thrown_ball = true;
   gameplay_meta_state.set_state_to_active_gameplay();
   //set_state(STATE_IN_SIMULATION);

   //}
   return;
}

bool Knockdown::is_cube_knocked_down(btRigidBody* cube, float initial_height, float threshold)
{
   if (!(cube))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::is_cube_knocked_down]: error: guard \"cube\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::is_cube_knocked_down]: error: guard \"cube\" not met");
   }
   //bool is_block_knocked_down(btRigidBody* block, float initial_height, float threshold = 0.8f)
   //{
      btVector3 position = cube->getCenterOfMassPosition();
      return position.getY() < (initial_height * threshold);
   //}
}

int Knockdown::count_cubes_knocked_down()
{
   if (!((cubes.size() == cube_initial_heights.size())))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::count_cubes_knocked_down]: error: guard \"(cubes.size() == cube_initial_heights.size())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::count_cubes_knocked_down]: error: guard \"(cubes.size() == cube_initial_heights.size())\" not met");
   }
   int cubes_knocked_down = 0;
   for (int i=0; i<(int)cubes.size(); i++)
   {
      if (is_cube_knocked_down(cubes[i].first, cube_initial_heights[i])) cubes_knocked_down++;
   }
   return cubes_knocked_down;
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
       convex_shape->addPoint(bt_vertex, false); // false avoids automatic recalculation of the AABB
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
   if (!((cube_index < (int)cubes.size())))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_cube_body_position_and_rotation]: error: guard \"(cube_index < (int)cubes.size())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_cube_body_position_and_rotation]: error: guard \"(cube_index < (int)cubes.size())\" not met");
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
   if (!((shape_index < (int)shapes.size())))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::capture_shape_body_position_and_rotation]: error: guard \"(shape_index < (int)shapes.size())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::capture_shape_body_position_and_rotation]: error: guard \"(shape_index < (int)shapes.size())\" not met");
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
   clear();
   // Cleanup
   //dynamics_world->removeRigidBody(sphere_body);
   //delete sphere_body->getMotionState();
   //delete sphere_body;
   //delete sphere_shape;

   /*
   dynamics_world->removeRigidBody(cube_body);
   delete cube_body->getMotionState();
   delete cube_body;
   */
   //delete cube_shape;

   //dynamics_world->removeRigidBody(ground_body);
   //delete ground_body->getMotionState();
   //delete ground_body;
   //delete ground_shape;

   // TODO: Delete properties of cubes

   // Destroy the rendering
   destroy_render();

   //delete dynamics_world;
   destroyed = true;
   return;
}

void Knockdown::set_camera_to_start_position()
{
   camera3d.stepout = AllegroFlare::Vec3D(0, 1.25, 16);
   camera3d.tilt = 0.65;
   camera3d.spin = 1.25;
   camera3d.near_plane = 0.25;
   camera3d.far_plane = 500.0;
   return;
}

void Knockdown::initialize_render()
{
   set_update_strategy(AllegroFlare::Screens::Base::UpdateStrategy::SEPARATE_UPDATE_AND_RENDER_FUNCS);

   // Setup the camera
   set_camera_to_start_position();
   //camera3d.stepout = AllegroFlare::Vec3D(0, 1.25, 16);
   //camera3d.tilt = 0.65;
   //camera3d.spin = 1.25;
   //camera3d.near_plane = 0.25;
   //camera3d.far_plane = 500.0;

   // Setup the model_bin
   model_bin.set_full_path(data_folder_path + "models");

   // Setup the bitmap_bin
   bitmap_bin.set_full_path(data_folder_path + "bitmaps");

   // Setup the font_bin
   font_bin.set_full_path(data_folder_path + "fonts");

   //rendering_setup = true;

   return;
}

void Knockdown::destroy_render()
{
   //model_bin
   model_bin.clear();

   //bitmap_bin
   bitmap_bin.clear();

   //font_bin
   font_bin.clear();
   return;
}

void Knockdown::primary_update_func(double time_now, double time_step)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::primary_update_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::primary_update_func]: error: guard \"initialized\" not met");
   }
   //AllegroFlare::Screens::Gameplay::primary_update_func(time_now, time_step);
   //throw std::runtime_error("jasdiofajsiodfjasdiof");
   if (get_gameplay_suspended()) return;

   //throw std::runtime_error("jasdiofajsiodfjasdiof");
   step_physics(time_step * 2); // Make the time step faster with * 2 since it's coming in as 1.0/60.0
   gameplay_meta_state.time_step_state(time_step);
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

void Knockdown::render_world()
{
   AllegroFlare::Model3D *sphere_model = model_bin.auto_get("unit_sphere-01.obj");
   AllegroFlare::Model3D *cube_model = model_bin.auto_get("centered_unit_cube-03.obj");
   //AllegroFlare::Model3D *shape_model = model_bin.auto_get("obscure_unit_tetrahedron-02.obj");
   AllegroFlare::Model3D *shape_model = model_bin.auto_get("fancy_dice-01.obj");
   //AllegroFlare::Model3D *shape_model = model_bin.auto_get("dice_v-01.obj");
   cube_model->set_texture(bitmap_bin.auto_get("dice_faces-04.png"));
   //shape_model->set_texture(get_bitmap_bin_ref().auto_get("obscure_unit_tetrahedron-02.png"));
   shape_model->set_texture(bitmap_bin.auto_get("fancy_dice-02.png"));
   shape_model->set_texture(bitmap_bin.auto_get("fancy_neopets_dice-01.png"));
   shape_model->set_texture(bitmap_bin.auto_get("6_dice_sides-01b.png"));



   // Create some scene "entity" variables
   AllegroFlare::Vec3D sphere_body_position;
   //AllegroFlare::Vec3D cube_body_position;
   AllegroFlare::Vec3D sphere_body_rotation;
   AllegroFlare::Vec3D cube_body_rotation;
   AllegroFlare::Placement3D sphere_body_placement;
   AllegroFlare::Placement3D cube_body_placement;

   std::vector<AllegroFlare::Vec3D> cube_body_positions;
   std::vector<AllegroFlare::Vec3D> cube_body_rotations;
   std::vector<AllegroFlare::Placement3D> cube_body_placements;
   int _num_cubes = num_cubes();
   cube_body_positions.resize(_num_cubes);
   cube_body_rotations.resize(_num_cubes);
   cube_body_placements.resize(_num_cubes);

   std::vector<AllegroFlare::Vec3D> shape_body_positions;
   std::vector<AllegroFlare::Vec3D> shape_body_rotations;
   std::vector<AllegroFlare::Placement3D> shape_body_placements;
   int _num_shapes = num_shapes();
   shape_body_positions.resize(_num_shapes);
   shape_body_rotations.resize(_num_shapes);
   shape_body_placements.resize(_num_shapes);



         // Synchronize the physics with the visual
         this->capture_sphere_body_position_and_rotation(&sphere_body_position, &sphere_body_rotation);
         //screen.capture_cube_body_position_and_rotation(&cube_body_position, &cube_body_rotation);
         //cube_body_placement.scale = {2, 2, 2};
         for (int i=0; i<_num_cubes; i++)
         {
            this->capture_cube_body_position_and_rotation(&cube_body_positions[i], &cube_body_rotations[i], i);
            cube_body_placements[i].position = cube_body_positions[i];
            cube_body_placements[i].rotation = cube_body_rotations[i];
            //cube_body_placements[i].scale = {2, 2, 2};
         }

         for (int i=0; i<_num_shapes; i++)
         {
            this->capture_shape_body_position_and_rotation(&shape_body_positions[i], &shape_body_rotations[i], i);
            shape_body_placements[i].position = shape_body_positions[i];
            shape_body_placements[i].rotation = shape_body_rotations[i];
            //cube_body_placements[i].scale = {2, 2, 2};
         }
         

         // Update the sphere and cube placements
         sphere_body_placement.position = sphere_body_position;
         sphere_body_placement.rotation = sphere_body_rotation;
         sphere_body_placement.scale = { 2, 2, 2 };




   // Setup the 3d scene projection
   camera3d.setup_projection_on(al_get_target_bitmap());


   {
         // Draw the sphere and cube objects
         sphere_body_placement.start_transform();
         sphere_model->draw();
         sphere_body_placement.restore_transform();



         int num_cubes = this->num_cubes();
         for (int i=0; i<num_cubes; i++)
         {
            ALLEGRO_TRANSFORM previous_transform;

            if (!al_get_current_transform()) return;
            al_copy_transform(&previous_transform, al_get_current_transform());

            //this->build_transform(&transform);

            //al_compose_transform(&transform, &previous_transform);
            //al_use_transform(&transform);
            
            ALLEGRO_TRANSFORM transform;
            BulletPhysics::Examples::Knockdown::build_transform_bullet(&transform, &cube_body_placements[i]);
            al_compose_transform(&transform, &previous_transform);
            al_use_transform(&transform);
            //cube_body_placement.start_transform();


            cube_model->draw();


            //cube_body_placement.restore_transform();
            al_use_transform(&previous_transform);
         }


         int num_shapes = this->num_shapes();
         for (int i=0; i<num_shapes; i++)
         {
            ALLEGRO_TRANSFORM previous_transform;

            if (!al_get_current_transform()) return;
            al_copy_transform(&previous_transform, al_get_current_transform());

            //this->build_transform(&transform);

            //al_compose_transform(&transform, &previous_transform);
            //al_use_transform(&transform);
            
            ALLEGRO_TRANSFORM transform;
            BulletPhysics::Examples::Knockdown::build_transform_bullet(&transform, &shape_body_placements[i]);
            al_compose_transform(&transform, &previous_transform);
            al_use_transform(&transform);
            //shape_body_placement.start_transform();


            shape_model->draw();


            //shape_body_placement.restore_transform();
            al_use_transform(&previous_transform);
         }
   }
   return;
}

void Knockdown::render_hud()
{
   hud_camera.setup_dimensional_projection(al_get_target_bitmap());
   {
      // Draw HUD
      {
         hud_camera.setup_dimensional_projection(al_get_target_bitmap());
         //cube_body_placement.position = cube_body_position;
         //camera.setup_projection_on(al_get_target_bitmap());
         //hud_camera.setup_projection_on(al_get_target_bitmap());
         int num_cubes_knocked_down = this->count_cubes_knocked_down();
         hud_camera.setup_dimensional_projection(al_get_target_bitmap());

         ALLEGRO_FONT *font = get_any_font(&font_bin);

         /* // Debugging
         //ALLEGRO_FONT *font = get_any_font(&font_bin);
         al_draw_multiline_textf(
            font,
            ALLEGRO_COLOR{1, 1, 1, 1},
            1920/2,
            80,
            1920,
            al_get_font_line_height(font),
            ALLEGRO_ALIGN_CENTER,
            "KNOCKED DOWN\n%d",
               num_cubes_knocked_down
         );
         */


            //ALLEGRO_FONT *font_x = get_font_bin_ref().auto_get("Oswald-Medium.ttf -262");
            ALLEGRO_FONT *font_x = font_bin.auto_get("Oswald-Medium.ttf -262");
         if (this->showing_final_score())
         {
            ALLEGRO_FONT *font = font_x;
            al_draw_multiline_textf(
               font,
               ALLEGRO_COLOR{0.4, 0.72, 0.8, 1},
               1920/2,
               1080/2 - al_get_font_line_height(font),
               1920,
               al_get_font_line_height(font),
               ALLEGRO_ALIGN_CENTER,
               "KNOCKED DOWN\n%d",
                  num_cubes_knocked_down
            );
         }



         if (this->showing_ready_banner())
         {
            ALLEGRO_FONT *font = font_x;
            al_draw_multiline_textf(
               font,
               ALLEGRO_COLOR{0.4, 0.72, 0.8, 1},
               1920/2,
               1080/2 - al_get_font_line_height(font) * 0.5,
               1920,
               al_get_font_line_height(font),
               ALLEGRO_ALIGN_CENTER,
               "READY"
                  //num_cubes_knocked_down
            );
         }


         if (this->showing_gameplay_instructions())
         {
            // For debugging, show state
            al_draw_multiline_textf(
               font,
               ALLEGRO_COLOR{1, 1, 1, 1},
               1920/2,
               1080-80*2,
               1920,
               al_get_font_line_height(font),
               ALLEGRO_ALIGN_CENTER,
               "Press SPACEBAR to launch ball"
            );
         }


         if (this->showing_press_any_key_to_continue_after_score_tally())
         {
            // For debugging, show state
            al_draw_multiline_textf(
               font,
               ALLEGRO_COLOR{1, 1, 1, 1},
               1920/2,
               1080-80*2,
               1920,
               al_get_font_line_height(font),
               ALLEGRO_ALIGN_CENTER,
               "Press any key to continue"
            );
         }


         // For DEVELOPMENT, show player_num_lives and inventory
         {
            std::stringstream inventory_items_str;
            for (auto &inventory_item : player_inventory_items)
            {
               inventory_items_str << inventory_item << " ";
            }

            // For debugging, show state
            al_draw_multiline_textf(
               font,
               ALLEGRO_COLOR{1, 1, 1, 1},
               300,
               100,
               1920,
               al_get_font_line_height(font),
               ALLEGRO_ALIGN_LEFT,
               "Player num lives: %d\n"
                  "Player inventory_items: %s",
                  player_num_lives,
                  inventory_items_str.str().c_str()
            );
         }



         /*
         // For debugging, show state
         al_draw_multiline_textf(
            font,
            ALLEGRO_COLOR{1, 1, 1, 1},
            1920/2,
            1080-80*2,
            1920,
            al_get_font_line_height(font),
            ALLEGRO_ALIGN_CENTER,
            "STATE: %s",
               this->get_current_state_name()
         );
         */
      }
   }




   // Setup the hud projection
   hud_camera.setup_dimensional_projection(al_get_target_bitmap());

   /*
   // For debugging, show state
   //ALLEGRO_FONT *font = font_bin["Oswald-Medium.ttf -262"];
   ALLEGRO_FONT *font = get_any_font(&font_bin);
   //get_any_font
   al_draw_multiline_textf(
      font,
      ALLEGRO_COLOR{1, 1, 1, 1},
      1920/2,
      1080-80*2,
      1920,
      al_get_font_line_height(font),
      ALLEGRO_ALIGN_CENTER,
      "STATE: %d",
         state
   );
   */
   return;
}

void Knockdown::primary_render_func()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::primary_render_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::primary_render_func]: error: guard \"initialized\" not met");
   }
   //AllegroFlare::Screens::Gameplay::primary_render_func();
   if (get_gameplay_suspended())
   {
      ALLEGRO_FONT *font = get_any_font(&font_bin);
      //get_any_font
      al_draw_multiline_text(
         font,
         ALLEGRO_COLOR{1, 1, 1, 1},
         1920/2,
         1080-80*2,
         1920,
         al_get_font_line_height(font),
         ALLEGRO_ALIGN_CENTER,
         "PAUSED"
      );
      return;
   }


   camera3d.spin -= 0.0125 * 0.25;


   // Render the world
   render_world();


   // Render the hud
   render_hud();


   return;
}

ALLEGRO_FONT* Knockdown::get_any_font(AllegroFlare::FontBin* font_bin, int size)
{
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::get_any_font]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::get_any_font]: error: guard \"font_bin\" not met");
   }
   if (!((size != 0)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Examples::Knockdown::get_any_font]: error: guard \"(size != 0)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Examples::Knockdown::get_any_font]: error: guard \"(size != 0)\" not met");
   }
   std::stringstream ss;
   ss << "Inter-Regular.ttf " << size;
   return font_bin->auto_get(ss.str());
}

bool Knockdown::showing_final_score()
{
   return gameplay_meta_state.showing_final_score();
      //is_state(STATE_TALLYING_SCORE)
      //|| is_state(STATE_SCORE_TALLIED_AND_PRESENTING)
      //|| is_state(STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);
}

bool Knockdown::showing_ready_banner()
{
   return gameplay_meta_state.showing_ready_banner();
   //return is_state(STATE_OPENING_SEQUENCE);
}

bool Knockdown::showing_gameplay_instructions()
{
   return gameplay_meta_state.showing_gameplay_instructions();
   //return is_state(STATE_WAITING_FOR_PLAYER_TO_THROW_BALL);
}

bool Knockdown::showing_press_any_key_to_continue_after_score_tally()
{
   return gameplay_meta_state.showing_press_any_key_to_continue_after_score_tally();
   //return is_state(STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);
}

bool Knockdown::waiting_for_player_input_to_continue()
{
   return gameplay_meta_state.waiting_for_player_input_to_continue();
   //return is_state(STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);
}

void Knockdown::advance_from_waiting_for_player_input_to_continue()
{
   return gameplay_meta_state.advance_from_waiting_for_player_input_to_continue();
   //return gameplay_meta_state.showing_final_score();
   //set_state(STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION);
}


} // namespace Examples
} // namespace BulletPhysics


