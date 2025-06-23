#pragma once

namespace app_domain
{
    struct Item;

    struct InventoryItemDetails
    {
        size_t Index = 0u;
        const Item& Item;
        std::uint32_t Count = 0u;
    };
}