

#include <BulletPhysics/GameplayMetaState/Basic.hpp>

#include <AllegroFlare/Logger.hpp>
#include <allegro5/allegro.h>
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
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
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


bool Basic::is_player_input_active()
{
   return true;
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
   state_changed_at = al_get_time();

   switch (state)
   {
      case STATE_WAITING_TO_START: {
      } break;

      case STATE_OPENING_SEQUENCE: {
         dip_to_black_opacity = 1.0f;
      } break;

      case STATE_WAITING_FOR_PLAYER_TO_THROW_BALL: {
      } break;

      case STATE_IN_SIMULATION: {
      } break;

      case STATE_TALLYING_SCORE: {
      } break;

      case STATE_SCORE_TALLIED_AND_PRESENTING: {
      } break;

      case STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE: {
      } break;

      case STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION: {
         dip_to_black_opacity = 0.0f;
      } break;

      case STATE_SCORE_TALLY_CLOSED_OUT: {
         dip_to_black_opacity = 1.0f;
         if (on_closed_out_func) on_closed_out_func();
         //clear();
         //reset();
      } break;

      /*
      case STATE_REVEALING:
      break;

      case STATE_AWAITING_USER_INPUT:
      break;

      case STATE_CLOSING_DOWN:
      break;
      */

      default: {
         AllegroFlare::Logger::throw_error(
            "BulletPhysics::GameplayMetaState::Basic::set_state",
            "Unable to handle case for state \"" + std::to_string(state) + "\""
         );
      } break;
   }

   //this->state = state;
   //state_changed_at = al_get_time();

   return;
}

void Basic::update_state(double time_now, double time_step)
{
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[BulletPhysics::GameplayMetaState::Basic::update_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[BulletPhysics::GameplayMetaState::Basic::update_state]: error: guard \"is_valid_state(state)\" not met");
   }
   float real_age = infer_current_state_real_age(time_now);

   switch (state)
   {
      case STATE_WAITING_TO_START: {
      } break;

      case STATE_OPENING_SEQUENCE: {
         dip_to_black_opacity -= 0.025f;
         if (real_age > 2.0)
         {
            dip_to_black_opacity = 0.0f;
            set_state(STATE_WAITING_FOR_PLAYER_TO_THROW_BALL);
         }
      } break;

      case STATE_WAITING_FOR_PLAYER_TO_THROW_BALL: {
      } break;

      case STATE_IN_SIMULATION: {
         if (real_age > 4.0) set_state(STATE_TALLYING_SCORE);
      } break;

      case STATE_TALLYING_SCORE: {
         if (real_age > 2.0) set_state(STATE_SCORE_TALLIED_AND_PRESENTING);
      } break;

      case STATE_SCORE_TALLIED_AND_PRESENTING: {
         if (real_age > 1.0) set_state(STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);
      } break;

      case STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE: {
      } break;

      case STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION: {
         dip_to_black_opacity += 0.025f;
         if (real_age > 1.0) set_state(STATE_SCORE_TALLY_CLOSED_OUT);
      } break;

      case STATE_SCORE_TALLY_CLOSED_OUT: {
         // TODO: Consider if "on_finished" is needed here
         dip_to_black_opacity = 1.0f;
      } break;

      /*
      case STATE_REVEALING:
      break;

      case STATE_AWAITING_USER_INPUT:
      break;

      case STATE_CLOSING_DOWN:
      break;
      */

      default:
         AllegroFlare::Logger::throw_error(
            "ClassName::update_state",
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
      STATE_WAITING_TO_START,
      STATE_OPENING_SEQUENCE,
      STATE_WAITING_FOR_PLAYER_TO_THROW_BALL,
      STATE_IN_SIMULATION,
      STATE_TALLYING_SCORE,
      STATE_SCORE_TALLIED_AND_PRESENTING,
      STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE,
      STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION,
      STATE_SCORE_TALLY_CLOSED_OUT,
      //STATE_REVEALING,
      //STATE_AWAITING_USER_INPUT,
      //STATE_CLOSING_DOWN,
   };
   return (valid_states.count(state) > 0);
}

bool Basic::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

float Basic::infer_current_state_real_age(float time_now)
{
   return (time_now - state_changed_at);
}

bool Basic::showing_final_score()
{
   return
      is_state(STATE_TALLYING_SCORE)
      || is_state(STATE_SCORE_TALLIED_AND_PRESENTING)
      || is_state(STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);
}

bool Basic::showing_ready_banner()
{
   return is_state(STATE_OPENING_SEQUENCE);
}

bool Basic::showing_gamplay_instructions()
{
   return is_state(STATE_WAITING_FOR_PLAYER_TO_THROW_BALL);
}

bool Basic::showing_press_any_key_to_continue_after_score_tally()
{
   return is_state(STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);
}

bool Basic::waiting_for_player_input_to_continue()
{
   return is_state(STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE);
}

void Basic::continue_from_waiting_for_player_input_to_continue()
{
   set_state(STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION);
}


} // namespace GameplayMetaState
} // namespace BulletPhysics


