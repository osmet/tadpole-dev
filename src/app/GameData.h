#pragma once
#include "CharacterData.h"
#include "InventoryData.h"

namespace app
{
	struct GameData
	{
		std::unordered_map<std::string, CharacterData> Characters;
		std::unordered_map<std::string, InventoryData> Inventories;
	};
}
