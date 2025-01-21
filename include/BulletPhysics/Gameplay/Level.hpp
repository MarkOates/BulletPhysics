#pragma once


#include <AllegroFlare/Levels/Base.hpp>
#include <string>


namespace BulletPhysics
{
   namespace Gameplay
   {
      class Level : public AllegroFlare::Levels::Base
      {
      public:
         static constexpr char* TYPE = (char*)"BulletPhysics/Gameplay/Level";

      private:
         std::string title;

      protected:


      public:
         Level();
         ~Level();

         void set_title(std::string title);
         std::string get_title() const;
      };
   }
}



