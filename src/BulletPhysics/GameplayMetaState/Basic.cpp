

#include <BulletPhysics/GameplayMetaState/Basic.hpp>

#include <AllegroFlare/Logger.hpp>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>


namespace BulletPhysics
{
namespace GameplayMetaState
{


Basic::Basic()
   : BulletPhysics::GameplayMetaState::Base(BulletPhysics::GameplayMetaState::Basic::TYPE)
   , on_closed_out_func({})
   , dip_to_black_opacity(0.0f)
   , state(GAMEPLAY_META_STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0)
   , accumulated_time_now(0.0)
{
}


Basic::~Basic()
{
}


void Basic::set_on_closed_out_func(std::function<void()> on_closed_out_func)
{
   this->on_closed_out_func = on_closed_out_func;
}


std::function<void()> Basic::get_on_closed_out_func() const
{
   return on_closed_out_func;
}


float Basic::get_dip_to_black_opacity() const
{
   return dip_to_black_opacity;
}


uint32_t Basic::get_state() const
{
   return state;
}


void Basic::start_opening_sequence()
{
   set_state(GAMEPLAY_META_STATE_OPENING_SEQUENCE);
   return;
}

bool Basic::is_player_input_active()
{
   return is_state(GAMEPLAY_META_STATE_WAITING_FOR_PLAYER_TO_THROW_BALL)
      || is_state(GAMEPLAY_META_STATE_IN_ACTIVE_GAMEPLAY)
      || is_state(GAMEPLAY_META_STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);

   //gameplay_meta_state.set_state_to_waiting_to_start();
   //GAMEPLAY_META_STATE_WAITING_TO_START
}

void Basic::set_state_to_loaded_and_waiting_to_start()
{
   set_state(GAMEPLAY_META_STATE_LOADED_AND_WAITING_TO_START);
   return;
}

void Basic::set_state_to_active_gameplay()
{
   set_state(GAMEPLAY_META_STATE_IN_ACTIVE_GAMEPLAY);
   return;
}

void Basic::set_state(uint32_t state, bool override_if_busy)
{
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::GameplayMetaState::Basic::set_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::GameplayMetaState::Basic::set_state]: error: guard \"is_valid_state(state)\" not met");
   }
   if (this->state == state) return;
   if (!override_if_busy && state_is_busy) return;
   uint32_t previous_state = this->state;

   this->state = state;
   state_changed_at = accumulated_time_now;

   switch (state)
   {
      case GAMEPLAY_META_STATE_LOADED_AND_WAITING_TO_START: {
      } break;

      case GAMEPLAY_META_STATE_OPENING_SEQUENCE: {
         dip_to_black_opacity = 1.0f;
      } break;

      case GAMEPLAY_META_STATE_WAITING_FOR_PLAYER_TO_THROW_BALL: {
      } break;

      case GAMEPLAY_META_STATE_IN_ACTIVE_GAMEPLAY: {
      } break;

      case GAMEPLAY_META_STATE_TALLYING_SCORE: {
      } break;

      case GAMEPLAY_META_STATE_SCORE_TALLIED_AND_PRESENTING: {
      } break;

      case GAMEPLAY_META_STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE: {
      } break;

      case GAMEPLAY_META_STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION: {
         dip_to_black_opacity = 0.0f;
      } break;

      case GAMEPLAY_META_STATE_SCORE_TALLY_CLOSED_OUT: {
         dip_to_black_opacity = 1.0f;
         if (on_closed_out_func) on_closed_out_func();
         //clear();
         //reset();
      } break;

      /*
      case GAMEPLAY_META_STATE_REVEALING:
      break;

      case GAMEPLAY_META_STATE_AWAITING_USER_INPUT:
      break;

      case GAMEPLAY_META_STATE_CLOSING_DOWN:
      break;
      */

      default: {
         AllegroFlare::Logger::throw_error(THIS_CLASS_AND_METHOD_NAME,
            "Unable to handle case for state \"" + std::to_string(state) + "\""
         );
      } break;
   }

   return;
}

void Basic::time_step_state(double time_step)
{
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::GameplayMetaState::Basic::time_step_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::GameplayMetaState::Basic::time_step_state]: error: guard \"is_valid_state(state)\" not met");
   }
   accumulated_time_now += time_step;
   double age = infer_current_state_age();

   switch (state)
   {
      case GAMEPLAY_META_STATE_LOADED_AND_WAITING_TO_START: {
      } break;

      case GAMEPLAY_META_STATE_OPENING_SEQUENCE: {
         dip_to_black_opacity -= 0.025f;
         if (age > 2.0)
         {
            dip_to_black_opacity = 0.0f;
            set_state(GAMEPLAY_META_STATE_WAITING_FOR_PLAYER_TO_THROW_BALL);
         }
      } break;

      case GAMEPLAY_META_STATE_WAITING_FOR_PLAYER_TO_THROW_BALL: {
      } break;

      case GAMEPLAY_META_STATE_IN_ACTIVE_GAMEPLAY: {
         if (age > 4.0) set_state(GAMEPLAY_META_STATE_TALLYING_SCORE);
      } break;

      case GAMEPLAY_META_STATE_TALLYING_SCORE: {
         if (age > 2.0) set_state(GAMEPLAY_META_STATE_SCORE_TALLIED_AND_PRESENTING);
      } break;

      case GAMEPLAY_META_STATE_SCORE_TALLIED_AND_PRESENTING: {
         if (age > 1.0) set_state(GAMEPLAY_META_STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);
      } break;

      case GAMEPLAY_META_STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE: {
      } break;

      case GAMEPLAY_META_STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION: {
         dip_to_black_opacity += 0.025f;
         if (age > 1.0) set_state(GAMEPLAY_META_STATE_SCORE_TALLY_CLOSED_OUT);
      } break;

      case GAMEPLAY_META_STATE_SCORE_TALLY_CLOSED_OUT: {
         // TODO: Consider if "on_finished" is needed here
         dip_to_black_opacity = 1.0f;
      } break;

      default:
         AllegroFlare::Logger::throw_error(THIS_CLASS_AND_METHOD_NAME,
            "Unable to handle case for state \"" + std::to_string(state) + "\""
         );
      break;
   }

   return;
}

bool Basic::is_valid_state(uint32_t state)
{
   std::set<uint32_t> valid_states =
   {
      GAMEPLAY_META_STATE_LOADED_AND_WAITING_TO_START,
      GAMEPLAY_META_STATE_OPENING_SEQUENCE,
      GAMEPLAY_META_STATE_WAITING_FOR_PLAYER_TO_THROW_BALL,
      GAMEPLAY_META_STATE_IN_ACTIVE_GAMEPLAY,
      GAMEPLAY_META_STATE_TALLYING_SCORE,
      GAMEPLAY_META_STATE_SCORE_TALLIED_AND_PRESENTING,
      GAMEPLAY_META_STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE,
      GAMEPLAY_META_STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION,
      GAMEPLAY_META_STATE_SCORE_TALLY_CLOSED_OUT,
   };
   return (valid_states.count(state) > 0);
}

std::string Basic::get_current_state_name()
{
   return get_state_name(state);
}

std::string Basic::get_state_name(uint32_t state)
{
   static const std::unordered_map<uint32_t, std::string> state_names =
   {
      { GAMEPLAY_META_STATE_LOADED_AND_WAITING_TO_START,
        "GAMEPLAY_META_STATE_LOADED_AND_WAITING_TO_START" },
      { GAMEPLAY_META_STATE_OPENING_SEQUENCE,
        "GAMEPLAY_META_STATE_OPENING_SEQUENCE" },
      { GAMEPLAY_META_STATE_WAITING_FOR_PLAYER_TO_THROW_BALL,
        "GAMEPLAY_META_STATE_WAITING_FOR_PLAYER_TO_THROW_BALL" },
      { GAMEPLAY_META_STATE_IN_ACTIVE_GAMEPLAY,
        "GAMEPLAY_META_STATE_IN_ACTIVE_GAMEPLAY" },
      { GAMEPLAY_META_STATE_TALLYING_SCORE,
        "GAMEPLAY_META_STATE_TALLYING_SCORE" },
      { GAMEPLAY_META_STATE_SCORE_TALLIED_AND_PRESENTING,
        "GAMEPLAY_META_STATE_SCORE_TALLIED_AND_PRESENTING" },
      { GAMEPLAY_META_STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE,
        "GAMEPLAY_META_STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE" },
      { GAMEPLAY_META_STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION,
        "GAMEPLAY_META_STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION" },
      { GAMEPLAY_META_STATE_SCORE_TALLY_CLOSED_OUT,
        "GAMEPLAY_META_STATE_SCORE_TALLY_CLOSED_OUT" },
   };

   auto it = state_names.find(state);
   if (it != state_names.end())
   {
      return it->second;
   }
   return "UNKNOWN_STATE";
}

bool Basic::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

double Basic::infer_current_state_age()
{
   return (accumulated_time_now - state_changed_at);
}

bool Basic::showing_final_score()
{
   return
      is_state(GAMEPLAY_META_STATE_TALLYING_SCORE)
      || is_state(GAMEPLAY_META_STATE_SCORE_TALLIED_AND_PRESENTING)
      || is_state(GAMEPLAY_META_STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);
}

bool Basic::showing_ready_banner()
{
   return is_state(GAMEPLAY_META_STATE_OPENING_SEQUENCE);
}

bool Basic::showing_gameplay_instructions()
{
   return is_state(GAMEPLAY_META_STATE_WAITING_FOR_PLAYER_TO_THROW_BALL);
}

bool Basic::showing_press_any_key_to_continue_after_score_tally()
{
   return is_state(GAMEPLAY_META_STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);
}

bool Basic::waiting_for_player_input_to_continue()
{
   return is_state(GAMEPLAY_META_STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);
}

void Basic::advance_from_waiting_for_player_input_to_continue()
{
   set_state(GAMEPLAY_META_STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION);
}


} // namespace GameplayMetaState
} // namespace BulletPhysics


