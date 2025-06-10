#pragma once

#include "ItemType.h"
#include "ItemRarity.h"

namespace app_domain
{
    struct Item
    {
        std::string Id;
        std::string Name;
        std::string IconTextureId;
        std::string Description;
        ItemType Type = ItemType::Other;
        bool IsStoryItem = false;
        ItemRarity Rarity = ItemRarity::Common;
        float Weight = 0.0f;
        std::int32_t Price = 0;
    };

    using ItemMap = std::unordered_map<std::string, Item>;
}