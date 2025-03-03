#pragma once


#include <BulletPhysics/GameplayMetaState/Base.hpp>
#include <cstdint>
#include <functional>
#include <string>


namespace BulletPhysics
{
   namespace GameplayMetaState
   {
      class Basic : public BulletPhysics::GameplayMetaState::Base
      {
      public:
         static constexpr char* TYPE = (char*)"BulletPhysics/GameplayMetaState/Basic";

      private:
         enum State
         {
            GAMEPLAY_META_STATE_UNDEF = 0,
            GAMEPLAY_META_STATE_LOADED_AND_WAITING_TO_START,
            GAMEPLAY_META_STATE_OPENING_SEQUENCE,
            GAMEPLAY_META_STATE_WAITING_FOR_PLAYER_TO_INITIATE_GAME_STARTING_ACTION,
            GAMEPLAY_META_STATE_IN_ACTIVE_GAMEPLAY,
            GAMEPLAY_META_STATE_TALLYING_SCORE,
            GAMEPLAY_META_STATE_SCORE_TALLIED_AND_PRESENTING,
            GAMEPLAY_META_STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE,
            GAMEPLAY_META_STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION,
            GAMEPLAY_META_STATE_SCORE_TALLY_CLOSED_OUT,
         };
         std::function<void()> on_closed_out_func;
         float dip_to_black_opacity;
         uint32_t state;
         bool state_is_busy;
         double state_changed_at;
         double accumulated_time_now;

      protected:


      public:
         Basic();
         virtual ~Basic();

         void set_on_closed_out_func(std::function<void()> on_closed_out_func);
         std::function<void()> get_on_closed_out_func() const;
         float get_dip_to_black_opacity() const;
         uint32_t get_state() const;
         void start_opening_sequence();
         virtual bool is_player_input_active() override;
         void set_state_to_loaded_and_waiting_to_start();
         void set_state_to_active_gameplay();
         void set_state(uint32_t state=GAMEPLAY_META_STATE_UNDEF, bool override_if_busy=false);
         void time_step_state(double time_step=(1.0/60.0f));
         static bool is_valid_state(uint32_t state=GAMEPLAY_META_STATE_UNDEF);
         std::string get_current_state_name();
         static std::string get_state_name(uint32_t state=GAMEPLAY_META_STATE_UNDEF);
         bool is_state(uint32_t possible_state=GAMEPLAY_META_STATE_UNDEF);
         double infer_current_state_age();
         bool showing_final_score();
         bool showing_ready_banner();
         bool showing_gameplay_instructions();
         bool showing_press_any_key_to_continue_after_score_tally();
         bool waiting_for_player_input_to_continue();
         void advance_from_waiting_for_player_input_to_continue();
      };
   }
}



