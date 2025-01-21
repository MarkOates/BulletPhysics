

#include <AllegroFlare/Runners/Complete.hpp>
#include <BulletPhysics/Game/Configurations/Main.hpp>
#include <allegro5/allegro.h>
#include <vector> // for parsing command line args
#include <algorithm> // for parsing command line args

int main(int argc, char **argv)
{
   std::vector<std::string> command_line_args;
   for (int i=1; i<argc; i++) command_line_args.push_back(argv[i]);
   bool contains_development_flag = std::count(command_line_args.begin(), command_line_args.end(), "--development");

   // TODO: improve this to return an error when an unrecognized flag is present

   BulletPhysics::Game::Configurations::Main main_configuration;

   std::string deployment_environment =
         contains_development_flag
         ? AllegroFlare::DeploymentEnvironment::ENVIRONMENT_DEVELOPMENT
         : AllegroFlare::DeploymentEnvironment::ENVIRONMENT_PRODUCTION;

   AllegroFlare::Runners::Complete().run(&main_configuration, deployment_environment);
   return 0;
}

