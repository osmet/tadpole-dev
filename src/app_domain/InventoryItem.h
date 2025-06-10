#pragma once

namespace app_domain
{
    struct InventoryItem
    {
        std::string ItemId;
        std::int32_t Count = 0;
    };
}