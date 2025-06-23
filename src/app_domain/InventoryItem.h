#pragma once

namespace app_domain
{
    struct InventoryItem
    {
        std::string ItemId;
        std::uint32_t Count = 0;
    };
}