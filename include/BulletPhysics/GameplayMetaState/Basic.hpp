#pragma once


#include <BulletPhysics/GameplayMetaState/Base.hpp>
#include <allegro5/allegro.h>
#include <cstdint>
#include <functional>


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
            STATE_UNDEF = 0,
            STATE_WAITING_TO_START,
            STATE_OPENING_SEQUENCE,
            STATE_WAITING_FOR_PLAYER_TO_THROW_BALL,
            STATE_IN_SIMULATION,
            STATE_TALLYING_SCORE,
            STATE_SCORE_TALLIED_AND_PRESENTING,
            STATE_SCORE_PRESENTED_AND_WAITING_FOR_PLAYER_TO_CONTINUE,
            STATE_CLOSING_OUT_SCORE_TALLY_PRESENTATION,
            STATE_SCORE_TALLY_CLOSED_OUT,
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
         void set_state(uint32_t state=STATE_UNDEF, bool override_if_busy=false);
         void update_state(double time_now=al_get_time(), double time_step=(1.0/60.0f));
         static bool is_valid_state(uint32_t state=STATE_UNDEF);
         bool is_state(uint32_t possible_state=STATE_UNDEF);
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



