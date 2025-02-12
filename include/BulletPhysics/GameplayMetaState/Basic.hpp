#pragma once


#include <BulletPhysics/GameplayMetaState/Base.hpp>
#include <allegro5/allegro.h>
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
            GAMEPLAY_META_STATE_WAITING_TO_START,
            GAMEPLAY_META_STATE_OPENING_SEQUENCE,
            GAMEPLAY_META_STATE_WAITING_FOR_PLAYER_TO_THROW_BALL,
            GAMEPLAY_META_STATE_IN_SIMULATION,
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

      protected:


      public:
         Basic();
         virtual ~Basic();

         void set_on_closed_out_func(std::function<void()> on_closed_out_func);
         std::function<void()> get_on_closed_out_func() const;
         float get_dip_to_black_opacity() const;
         uint32_t get_state() const;
         virtual bool is_player_input_active() override;
         void set_state(uint32_t state=GAMEPLAY_META_STATE_UNDEF, bool override_if_busy=false);
         void update_state(double time_step=(1.0/60.0f), double time_now=al_get_time());
         static bool is_valid_state(uint32_t state=GAMEPLAY_META_STATE_UNDEF);
         std::string get_state_name();
         bool is_state(uint32_t possible_state=GAMEPLAY_META_STATE_UNDEF);
         double infer_current_state_real_age(double time_now=al_get_time());
         bool showing_final_score();
         bool showing_ready_banner();
         bool showing_gamplay_instructions();
         bool showing_press_any_key_to_continue_after_score_tally();
         bool waiting_for_player_input_to_continue();
         void continue_from_waiting_for_player_input_to_continue();
      };
   }
}



