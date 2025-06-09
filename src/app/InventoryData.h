#pragma once

#include "InventoryItemData.h"

namespace app
{
    struct InventoryData
    {
        std::string Id;
        std::int32_t CurrentMoney = 0;
        std::vector<InventoryItemData> Items;
    };

    using InventoryDataMap = std::unordered_map<std::string, InventoryData>;
}