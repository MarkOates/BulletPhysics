#pragma once


#include <string>


namespace BulletPhysics
{
   namespace GameplayMetaState
   {
      class Base
      {
      public:
         static constexpr char* TYPE = (char*)"BulletPhysics/GameplayMetaState/Base";

      private:
         std::string type;

      protected:


      public:
         Base(std::string type=BulletPhysics::GameplayMetaState::Base::TYPE);
         virtual ~Base();

         std::string get_type() const;
         virtual bool is_player_input_active();
         bool is_type(std::string possible_type="");
      };
   }
}



