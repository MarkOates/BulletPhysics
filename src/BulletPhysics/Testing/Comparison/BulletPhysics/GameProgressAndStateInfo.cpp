

#include <BulletPhysics/Testing/Comparison/BulletPhysics/GameProgressAndStateInfo.hpp>


namespace BulletPhysics
{


bool operator==(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object)
{
   if (object.get_player_inventory_items() != other_object.get_player_inventory_items()) return false;
   return true;
}


bool operator!=(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object)
{
   return !(object == other_object);
}


void PrintTo(const GameProgressAndStateInfo& object, ::std::ostream* os)
{
   *os << "GameProgressAndStateInfo(";

   // player_inventory_items
   *os << "player_inventory_items: {";
   for (auto &player_inventory_item : object.get_player_inventory_items())
   {
      *os << "\"" << player_inventory_item << "\", ";
   }
   *os << "}, "

       << ")";
}


}


