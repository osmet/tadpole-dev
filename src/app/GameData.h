#pragma once
#include "CharacterData.h"
#include "InventoryData.h"

namespace app
{
    struct GameData
    {
        CharacterDataMap Characters;
        InventoryDataMap Inventories;
    };
}