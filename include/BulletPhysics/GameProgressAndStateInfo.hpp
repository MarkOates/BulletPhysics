#pragma once


#include <AllegroFlare/GameProgressAndStateInfos/Base.hpp>
#include <string>
#include <vector>


namespace BulletPhysics
{
   class GameProgressAndStateInfo : public AllegroFlare::GameProgressAndStateInfos::Base
   {
   public:
      static constexpr char* TYPE = (char*)"BulletPhysics/GameProgressAndStateInfo";

   private:
      std::vector<std::string> player_inventory_items;
      int player_num_lives;

   protected:


   public:
      GameProgressAndStateInfo();
      virtual ~GameProgressAndStateInfo();

      void set_player_inventory_items(std::vector<std::string> player_inventory_items);
      void set_player_num_lives(int player_num_lives);
      std::vector<std::string> get_player_inventory_items() const;
      int get_player_num_lives() const;
      std::vector<std::string> &get_player_inventory_items_ref();
      int &get_player_num_lives_ref();
      void add_player_inventory_item(std::string item_identifier="[unset-item_identifier]");
      virtual std::string export_to_string() override;
      virtual void import_from_string(std::string data_string="[unset-data_string]") override;
   };
}



