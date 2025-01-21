#pragma once


#include <lib/nlohmann/json.hpp>
#include <BulletPhysics/GameProgressAndStateInfo.hpp>


namespace BulletPhysics
{
  void to_json(nlohmann::json& j, const GameProgressAndStateInfo& object);
  void from_json(const nlohmann::json& j, GameProgressAndStateInfo& object);
}


