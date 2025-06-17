#pragma once

namespace app_domain
{
    enum class ItemCategory : std::int8_t
    {
        All = 0,
        Misc = 1,
        Equipment = 2,
        BooksAndKeys = 3,
        Consumables = 4,
        ScrollsAndTools = 5,
    };
}