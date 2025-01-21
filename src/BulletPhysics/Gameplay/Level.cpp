

#include <BulletPhysics/Gameplay/Level.hpp>




namespace BulletPhysics
{
namespace Gameplay
{


Level::Level()
   : AllegroFlare::Levels::Base(BulletPhysics::Gameplay::Level::TYPE)
   , title("[unset-title]")
{
}


Level::~Level()
{
}


void Level::set_title(std::string title)
{
   this->title = title;
}


std::string Level::get_title() const
{
   return title;
}




} // namespace Gameplay
} // namespace BulletPhysics


