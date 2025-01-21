#pragma once


#include <BulletPhysics/GameProgressAndStateInfo.hpp>
#include <ostream>


namespace BulletPhysics
{
   bool operator==(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object);
   bool operator!=(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object);
   void PrintTo(const GameProgressAndStateInfo& object, ::std::ostream* os);
}


