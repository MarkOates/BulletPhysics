

#include <BulletPhysics/GameplayMetaState/Base.hpp>




namespace BulletPhysics
{
namespace GameplayMetaState
{


Base::Base(std::string type)
   : type(type)
{
}


Base::~Base()
{
}


std::string Base::get_type() const
{
   return type;
}


bool Base::is_player_input_active()
{
   // This needs to be overridden in the base class
   return true;
}

bool Base::is_type(std::string possible_type)
{
   return (possible_type == get_type());
}


} // namespace GameplayMetaState
} // namespace BulletPhysics


