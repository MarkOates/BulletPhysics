

#include <BulletPhysics/Game/Configurations/Main.hpp>

#include <AllegroFlare/Achievement.hpp>
#include <AllegroFlare/DialogTree/NodeBankFactory.hpp>
#include <AllegroFlare/EventNames.hpp>
#include <AllegroFlare/GameEventDatas/AchievementUnlocked.hpp>
#include <AllegroFlare/GameProgressAndStateInfos/Base.hpp>
#include <AllegroFlare/Runners/Complete.hpp>
#include <BulletPhysics/Gameplay/Level.hpp>
#include <BulletPhysics/Gameplay/Screen.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>


namespace BulletPhysics
{
namespace Game
{
namespace Configurations
{


Main::Main()
   : AllegroFlare::GameConfigurations::Complete(BulletPhysics::Game::Configurations::Main::TYPE)
   , primary_gameplay_screen(nullptr)
   , primary_gameplay_subscreen(nullptr)
   , pause_screen(nullptr)
{
}


Main::~Main()
{
}


std::string Main::app_icon_1024_filename()
{
   return "icons/allegro-flare-generic-icon-1024.png";
}

std::string Main::app_title()
{
   return "Untitled Game";
}

std::string Main::title_screen_title()
{
   return "Untitled Game";
}

std::string Main::primary_display_icon_filename()
{
   return "icons/allegro-flare-generic-icon-1024.png";
}

std::vector<std::tuple<std::string, AllegroFlare::Achievement*, bool, bool>> Main::build_achievements()
{
   return {};
}

AllegroFlare::Screens::Gameplay* Main::create_primary_gameplay_screen(AllegroFlare::Runners::Complete* runner)
{
   if (!(runner))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Game::Configurations::Main::create_primary_gameplay_screen]: error: guard \"runner\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Game::Configurations::Main::create_primary_gameplay_screen]: error: guard \"runner\" not met");
   }
   // TODO: Find some way to remove the Runners::Complete dependency, consider injecting
   // the bin dependencies individually into this method

   BulletPhysics::Gameplay::Screen *result = new BulletPhysics::Gameplay::Screen;

   result->set_data_folder_path(runner->get_framework()->get_data_folder_path());
   result->set_asset_studio_database(&runner->get_framework()->get_asset_studio_database_ref());
   result->set_font_bin(runner->get_font_bin());
   result->set_bitmap_bin(runner->get_bitmap_bin());
   result->set_model_bin(runner->get_model_bin());
   result->set_event_emitter(runner->get_event_emitter());
   result->initialize();

   primary_gameplay_screen = result;

   return result;
}

AllegroFlare::Screens::Subscreen::Screen* Main::create_primary_gameplay_subscreen(AllegroFlare::Runners::Complete* runner)
{
   if (!((!primary_gameplay_subscreen)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Game::Configurations::Main::create_primary_gameplay_subscreen]: error: guard \"(!primary_gameplay_subscreen)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Game::Configurations::Main::create_primary_gameplay_subscreen]: error: guard \"(!primary_gameplay_subscreen)\" not met");
   }
   AllegroFlare::EventEmitter* event_emitter = runner->get_event_emitter();
   AllegroFlare::BitmapBin* bitmap_bin = runner->get_bitmap_bin();
   AllegroFlare::FontBin* font_bin = runner->get_font_bin();

   // Build the subscreen_element
   AllegroFlare::Screens::Subscreen::Element *subscreen_element = new AllegroFlare::Screens::Subscreen::Element;
   subscreen_element->set_bitmap_bin(bitmap_bin);
   subscreen_element->set_font_bin(font_bin);
   subscreen_element->initialize();

   // Build and assemble the subscreen
   primary_gameplay_subscreen = new AllegroFlare::Screens::Subscreen::Screen;
   primary_gameplay_subscreen->set_event_emitter(event_emitter);
   primary_gameplay_subscreen->set_bitmap_bin(bitmap_bin);
   primary_gameplay_subscreen->set_font_bin(font_bin);
   primary_gameplay_subscreen->set_subscreen_element(subscreen_element);

   // Set the exit screen behavior
   primary_gameplay_subscreen->set_on_exit_callback_func(
      [event_emitter](AllegroFlare::Screens::Subscreen::Screen* pause_screen, void* user_data){
         event_emitter->emit_router_event(
            AllegroFlare::Routers::Standard::EVENT_UNPAUSE_GAME,
            nullptr,
            al_get_time()
         );
      }
   );
   primary_gameplay_subscreen->set_on_exit_callback_func_user_data(nullptr);

   primary_gameplay_subscreen->initialize();

   return primary_gameplay_subscreen;
}

AllegroFlare::Screens::PauseScreen* Main::create_pause_screen(AllegroFlare::Runners::Complete* runner)
{
   if (!((!pause_screen)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Game::Configurations::Main::create_pause_screen]: error: guard \"(!pause_screen)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Game::Configurations::Main::create_pause_screen]: error: guard \"(!pause_screen)\" not met");
   }
   AllegroFlare::FontBin* font_bin = runner->get_font_bin();
   AllegroFlare::BitmapBin* bitmap_bin = runner->get_bitmap_bin();
   AllegroFlare::EventEmitter* event_emitter = runner->get_event_emitter();

   std::vector<std::pair<std::string, std::string>> menu_options = {
      { "Resume", "resume_gameplay" },
      { "Exit to Title Screen", "exit_to_title_screen" },
   };

   // NOTE: No pause screen is used in this game, however, a pause screen is needed because upstream in the system
   // a  cannot be registered as a screen. // TODO: Fix this
   pause_screen = new AllegroFlare::Screens::PauseScreen;
   pause_screen->set_font_bin(font_bin);
   pause_screen->set_bitmap_bin(bitmap_bin);
   pause_screen->set_event_emitter(event_emitter);
   pause_screen->set_menu_options(menu_options);

   //pause_screen->set_foreground(shared_foreground);
   //pause_screen->set_background(shared_background);
   // TODO: Configure menu items
   // TODO: Add actions to menu items

   pause_screen->set_on_menu_choice_callback_func(
      [event_emitter](AllegroFlare::Screens::PauseScreen* pause_screen, void* user_data){
         // TODO: Perform different action depending on selected menu item
         // TODO: Use event emitter from pause_screen

         std::string current_menu_option_value = pause_screen->infer_current_menu_option_value();
         if (current_menu_option_value == "resume_gameplay")
         {
            event_emitter->emit_router_event(
               AllegroFlare::Routers::Standard::EVENT_UNPAUSE_GAME,
               nullptr,
               al_get_time()
            );
         }
         else if (current_menu_option_value == "exit_to_title_screen")
         {
            // TODO: Add a "are you sure?" step here
            event_emitter->emit_router_event(
               AllegroFlare::Routers::Standard::EVENT_ACTIVATE_TITLE_SCREEN,
               nullptr,
               al_get_time()
            );
         }
         else
         {
            AllegroFlare::Logger::throw_error(
               "AllegroFlare::GameConfigurations::Base::create_pause_screen"
               "While in the \"on_menu_choice_callback_func\", there was no option to handle the menu value \""
                  + current_menu_option_value + "\"."
            );
         }
      }
   );
   pause_screen->set_on_menu_choice_callback_func_user_data(nullptr);

   // Set the exit screen behavior
   pause_screen->set_on_exit_callback_func(
      [event_emitter](AllegroFlare::Screens::PauseScreen* pause_screen, void* user_data){
         event_emitter->emit_router_event(
            AllegroFlare::Routers::Standard::EVENT_UNPAUSE_GAME,
            nullptr,
            al_get_time()
         );
      }
   );
   pause_screen->set_on_exit_callback_func_user_data(nullptr);

   return pause_screen;
}

void Main::handle_primary_gameplay_screen_paused()
{
   if (!(primary_gameplay_screen))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Game::Configurations::Main::handle_primary_gameplay_screen_paused]: error: guard \"primary_gameplay_screen\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Game::Configurations::Main::handle_primary_gameplay_screen_paused]: error: guard \"primary_gameplay_screen\" not met");
   }
   // TODO: Add tests for this behavior
   // TODO: Consider having and using event_emitter from self
   AllegroFlare::EventEmitter* event_emitter = primary_gameplay_screen->get_event_emitter();
   primary_gameplay_screen->suspend_gameplay();
   event_emitter->emit_router_event(
      AllegroFlare::Routers::Standard::EVENT_ACTIVATE_PAUSE_SCREEN,
      //AllegroFlare::Routers::Standard::EVENT_ACTIVATE_PRIMARY_GAMEPLAY_SUBSCREEN,
      nullptr,
      al_get_time()
   );
   return;
}

void Main::handle_primary_gameplay_screen_unpaused()
{
   if (!(primary_gameplay_screen))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Game::Configurations::Main::handle_primary_gameplay_screen_unpaused]: error: guard \"primary_gameplay_screen\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Game::Configurations::Main::handle_primary_gameplay_screen_unpaused]: error: guard \"primary_gameplay_screen\" not met");
   }
   // TODO: Add tests for this behavior
   // TODO: Consider having and using event_emitter from self
   AllegroFlare::EventEmitter* event_emitter = primary_gameplay_screen->get_event_emitter();
   primary_gameplay_screen->resume_suspended_gameplay();
   event_emitter->emit_router_event(
      AllegroFlare::Routers::Standard::EVENT_ACTIVATE_PRIMARY_GAMEPLAY_SCREEN,
      nullptr,
      al_get_time()
   );
   return;
}

std::vector<AllegroFlare::Elements::StoryboardPages::Base *> Main::create_intro_logos_storyboard_pages()
{
   //AllegroFlare::StoryboardPageFactory page_factory;
   //page_factory.set_font_bin(font_bin);
   //page_factory.set_bitmap_bin(bitmap_bin);
   //page_factory.set_model_bin(model_bin);
   //page_factory.create_clubcatt_logo_page(),
   //page_factory.create_image_page(bitmap_bin->operator[]("clubcatt-website-01.jpg")),
   return {};
}

std::vector<AllegroFlare::Elements::StoryboardPages::Base *> Main::create_intro_storyboard_pages()
{
   //AllegroFlare::Logger::throw_error(
      //"AllegroFlare::GameConfigurations::Base::create_intro_storyboard_pages",
      //"Not implemented in the base class. This method must be implemented in the derived class"
   //);
   //AllegroFlare::StoryboardPageFactory page_factory;
   //page_factory.set_font_bin(font_bin);
   //page_factory.set_bitmap_bin(bitmap_bin);
   //page_factory.set_model_bin(model_bin);

   //std::vector<AllegroFlare::Elements::StoryboardPages::Base *> result =
   //{
      //page_factory.create_image_with_advancing_text_page(
         //"storyboard-1-01-1165x500.png",
         //"Once upon a time, in a magical kingdom ruled by a wise and just queen, a young hero sets out on a "
            //"journey to prove himself and save his people from a terrible curse."
      //),
      //page_factory.create_image_with_advancing_text_page(
         //"storyboard-2-01-1165x500.png",
         //"With the help of his trusty sidekick and a band of unlikely allies, he must navigate treacherous "
            //"terrain and battle fierce foes."
      //),
      //page_factory.create_advancing_text_page(
        //"And achieve his goal to save the kingdom."
      //),
   //};
   return {};
}

void Main::handle_arbitrary_storyboard_screen_finished()
{
   AllegroFlare::Logger::throw_error(
      "AllegroFlare::GameConfigurations::Base::handle_arbitrary_storyboard_screen_finished",
      "Not implemented in the base class. This method must be implemented in the derived class"
   );
   // NOTE: To use, typically:
   //router->emit_route_event(
      //AllegroFlare::Routers::Standard::EVENT_ACTIVATE_LEVEL_SELECT_SCREEN,
      //nullptr,
      //al_get_time()
   //);
   return;
}

std::vector<AllegroFlare::Elements::StoryboardPages::Base *> Main::create_arbitrary_storyboard_pages_by_identifier(std::string identifier)
{
   // NOTE: At the time of this writing, identifier is not properly wired up, you will need to find
   // a way to transmit the identifier you'd like to use. In the past, an identifier is stored
   // at the point where the AllegroFlare::Routers::Standard::EVENT_ACTIVATE_ARBITRARY_STORYBOARD_SCREEN route
   // event is emitted, and is somehow retrieved here, or, this method dispatches out to the object/class where the
   // event was emitted.
   // Ideally, you would wire up the identifier to be passed along here. Probably as a new
   // AllegroFlare::RouteEventDatas::ActivateArbitraryStoryboardScreen or similar
   return {};
}

std::vector<std::pair<std::string, std::string>> Main::build_title_screen_menu_options()
{
   //AllegroFlare::Logger::throw_error(
      //"AllegroFlare::GameConfigurations::Base::build_title_screen_menu_options",
         //"Not implemented in the base class. This method must be implemented in the derived class"
   //);
   std::vector<std::pair<std::string, std::string>> options = {
      //{ "Continue",          "continue_from_last_save" },       // TODO: If game session is saved and valid
      //{ "Load a Saved Game", "goto_load_a_saved_game_screen" }, // TODO: If game session is saved and valid,
                                                                // and the game supports save slots
      { "Start New Game",    "start_new_game" },                // TODO: If the game session has not begun
      //{ "Achievements",      "goto_achievements_screen" },
      //{ "Settings",          "goto_settings_screen" },
      //{ "Version",           "goto_version_screen" },
      //{ "Credits",           "goto_credits_screen" },           // TODO: If game has been won
      { "Quit",              "quit" },
   };
   return options;
}

void Main::load_audio_controller(AllegroFlare::AudioController* audio_controller)
{
   if (!(audio_controller))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Game::Configurations::Main::load_audio_controller]: error: guard \"audio_controller\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Game::Configurations::Main::load_audio_controller]: error: guard \"audio_controller\" not met");
   }
   // An example of how to load a sound effect track:
   audio_controller->set_and_load_sound_effect_elements({
      //{ "menu_move", { "ci-menu_move-01.ogg", false, "restart" } },
      //{ "menu_select", { "ci-menu_choice-01.ogg", false, "restart" } },
   });

   // How to play a sound effect:
   // event_emitter->emit_play_music_track_event("menu_select");

   //// An example of how to load a music track:
   //audio_controller.set_and_load_music_track_elements({
      ////{ "intro_music", { "wanderer-01.ogg", true, "ignore" } },
   //});

   // How to play a music track:
   // event_emitter->emit_play_music_track_event("intro_music");
   return;
}

void Main::load_last_played_session_or_start_new(AllegroFlare::GameSession* game_session)
{
   if (!(game_session))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Game::Configurations::Main::load_last_played_session_or_start_new]: error: guard \"game_session\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Game::Configurations::Main::load_last_played_session_or_start_new]: error: guard \"game_session\" not met");
   }
   // TODO: This method
   //AllegroFlare::GameProgressAndStateInfos::Base *game_progress_and_state_info =
     //game_session->get_game_progress_and_state_info();
   return;
}

void Main::setup_new_game_progress_and_state_info(AllegroFlare::GameSession* game_session)
{
   if (!(game_session))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::Game::Configurations::Main::setup_new_game_progress_and_state_info]: error: guard \"game_session\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::Game::Configurations::Main::setup_new_game_progress_and_state_info]: error: guard \"game_session\" not met");
   }
   // TODO: This method
   //AllegroFlare::Logger::throw_error(
      //"AllegroFlare::GameConfigurations::Base::setup_new_game_progress_and_state_info",
      //"Not implemented in the base class. This method must be implemented in the derived class"
   //);
   //AllegroFlare::GameProgressAndStateInfos::Base *game_progress_and_state_info =
     //game_session->get_game_progress_and_state_info();
   return;
}

std::vector<AllegroFlare::LoadASavedGame::SaveSlots::Base*> Main::build_save_slots_for_load_a_saved_game_screen()
{
   /*
   std::vector<AllegroFlare::LoadASavedGame::SaveSlots::Base*> result = {
      new AllegroFlare::LoadASavedGame::SaveSlots::Empty(), // TODO: Fill this list with items from save file
      new AllegroFlare::LoadASavedGame::SaveSlots::Empty(),
      new AllegroFlare::LoadASavedGame::SaveSlots::Empty(),
   };
   return result;
   */
   return {};
}

void Main::handle_game_event(AllegroFlare::GameEvent* game_event)
{
   //// TODO: Handle top-level game events here
   //if (game_event->is_type(AllegroFlare::GameEventDatas::ScreenActivated::NAME))
   //{
      //AllegroFlare::GameEventDatas::ScreenActivated* as =
        //static_cast<AllegroFlare::GameEventDatas::ScreenActivated*>(game_event->get_data());

      //// TODO: Handle game-specific logic for a after a screen switch
   //}
   if (game_event->get_type() == AllegroFlare::GameEventDatas::AchievementUnlocked::NAME)
   {
      if (game_event->get_data()->is_type(AllegroFlare::GameEventDatas::AchievementUnlocked::TYPE))
      {
         AllegroFlare::GameEventDatas::AchievementUnlocked *as =
            static_cast<AllegroFlare::GameEventDatas::AchievementUnlocked *>(game_event->get_data());
         // TODO: Handle saving progress of achievements. Something like below.
         // See this guide:
         // https://docs.google.com/document/d/1QdDVa6giZOmGbF61dgom1ZJ_Ev5OFvZEM2Bc453RjGw/edit
         //mark_achievement_as_unlocked_and_save_progress(as->get_achievement_identifier());
      }
      else
      {
         throw std::runtime_error("Gameplay/Runner/game_event_func: unexpected AchievementUnlocked event data type");
      }
   }
   return;
}

void Main::continue_from_last_save()
{
   // TODO: Consider adding this method as part of the standard system
   return;
}

std::vector<std::pair<std::string, std::string>> Main::build_level_list_for_level_select_screen_by_identifier(std::string identifier)
{
   std::vector<std::pair<std::string, std::string>> result = {
      //{ "Forest Village 1", "forest_village_1" },
      //{ "Forest Village 2", "forest_village_2" },
      //{ "Forest", "forest_1" },
      //{ "Crystal Cave", "crystal_cave_1" },
      //{ "Desert Town", "desert_town_3" },
      //{ "Town 2", "town_2" },
      //{ "Cave 1", "cave_1" },
      //{ "Town 1", "town_1" },
   };
   return result;
}

AllegroFlare::DialogTree::NodeBank Main::build_dialog_bank_by_identifier(std::string identifier)
{
   // TODO: Test this contains the expected nodes
   AllegroFlare::DialogTree::NodeBank result_node_bank;

   // TODO: Consider joining the system nodes outside of the LevelFactory
   AllegroFlare::DialogTree::NodeBank system_node_bank =
      AllegroFlare::DialogTree::NodeBankFactory::build_common_system_dialogs_node_bank();
   result_node_bank.merge(&system_node_bank);

   return result_node_bank;
}

AllegroFlare::Levels::Base* Main::load_level_by_identifier(std::string identifier)
{
   // TODO: Replace void* with a better type (once the design is clear)
   BulletPhysics::Gameplay::Level *result = new BulletPhysics::Gameplay::Level();

   std::map<std::string, std::function<void()>> items_map = {
      { "", [](){
         AllegroFlare::Logger::warn_from(
            "BulletPhysics::GameConfigurations::Main::load_level_by_identifier",
            "Loading a completely blank empty level."
         );
         // TODO: Add warning: loading an empty level
      }},
      { "forest_village_1", [](){
         //result->set_background_image_identifier("maps/forest-village-1.png");
      }},
      //{ "forest_village_2", [](){
         //result->set_background_image_identifier("maps/forest-village-2.png");
      //}},
      //{ "forest_1", [result](){
         //result->set_background_image_identifier("maps/forest-1.png");
      //}},
      //{ "crystal_cave_1", [result](){
         //result->set_background_image_identifier("maps/crystal-cave-1.png");
      //}},
      //{ "desert_town_3", [result](){
         //result->set_background_image_identifier("maps/desert-town-3.png");
      //}},
      //{ "town_2", [result](){
         //result->set_background_image_identifier("maps/town-2.png");
      //}},
      //{ "cave_1", [result](){
         //result->set_background_image_identifier("maps/rpg-fit-backgrounds-ex-101.png");
      //}},
      //{ "town_1", [result](){
         //result->set_background_image_identifier("maps/rpg-fit-backgrounds-x2-01.png");
      //}},
   };

   // locate and call the function to handle the item
   if (items_map.count(identifier) == 0)
   {
      bool item_handled = false;
      //if (unrecognized_key_callback_func)
      //{
         //item_handled = unrecognized_key_callback_func();
      //}

      if (!item_handled)
      {
         // item not found
         std::stringstream error_message;
         error_message << "[BulletPhysics::Game::Configurations::Main::load_level]: error: Cannot load the item with the identifier \""
                       << identifier << "\", it does not exist.";
         throw std::runtime_error(error_message.str());
      }
   }
   else
   {
      // call the item
      items_map[identifier]();
   }

   return result;
}


} // namespace Configurations
} // namespace Game
} // namespace BulletPhysics


