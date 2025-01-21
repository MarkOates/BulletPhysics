

#include <BulletPhysics/JSONLoaders/BulletPhysics/GameProgressAndStateInfo.hpp>

#include <AllegroFlare/Logger.hpp>


namespace BulletPhysics
{


void to_json(nlohmann::json& j, const GameProgressAndStateInfo& v)
{
   j = nlohmann::json{
      { "player_inventory_items", v.get_player_inventory_items() },
   };
}


void from_json(const nlohmann::json& j, GameProgressAndStateInfo& v)
{
   // player_inventory_items
   if (!j.contains("player_inventory_items"))
   {
      AllegroFlare::Logger::throw_error(
         "When loading JSON, expecting to find \"player_inventory_items\" but it does not exist."
      );
   }
   j.at("player_inventory_items").get_to(v.get_player_inventory_items_ref());
}


} // namespace BulletPhysics


