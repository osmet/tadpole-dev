#pragma once

#include "ItemCategory.h"

namespace app_domain
{
    // IMPORTANT: Update ItemTypeHelper::GetItemCategory if ItemType changes
    enum class ItemType : std::int8_t
    {
        // Misc (0–10)
        Misc = 0,
        Valuable = 1,

        // Books and Keys (11-20)
        Book = 11,
        Note = 12,
        Key = 13,

        // Equipment (21–40)
        Armor = 21,
        Clothing = 22,
        Footwear = 23,
        Handwear = 24,
        Headwear = 25,
        Amulet = 26,
        Ring = 27,
        Weapon = 28,
        Shield = 29,

        // Consumables (41–50)
        Food = 41,
        Potion = 42,
        Elixir = 43,

        // Scrolls and Tools (51-60)
        Scroll = 51,
        Tool = 52,
        Bomb = 53
    };

    class ItemTypeHelper
    {
    public:
        static ItemType FromString(const std::string& str);
        static std::string ToString(ItemType type);

        static ItemCategory GetItemCategory(ItemType type);
    };
}