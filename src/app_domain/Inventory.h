#pragma once

#include "InventoryItem.h"

namespace app_domain
{
    struct Inventory
    {
        std::string Id;
        std::uint32_t CurrentMoney = 0;
        std::vector<InventoryItem> Items;
    };

    using InventoryMap = std::unordered_map<std::string, Inventory>;
}