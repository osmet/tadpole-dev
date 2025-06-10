#pragma once

#include "ItemCategory.h"
#include <magic_enum/magic_enum.hpp>

namespace app_domain
{
    enum class ItemType : std::int8_t
    {
        // Misc (0–10)
        Other = 0,
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

        // Consumables (41–60)
        Food = 41,
        Potion = 42,
        Elixir = 43,
        Scroll = 44,
        Tool = 45,
        Bomb = 46
    };

    class ItemTypeHelper
    {
    public:
        constexpr ItemCategory GetItemCategory(ItemType type)
        {
            if (type >= ItemType::Other && type <= ItemType::Valuable)
                return ItemCategory::Misc;

            if (type >= ItemType::Book && type <= ItemType::Key)
                return ItemCategory::BooksAndKeys;

            if (type >= ItemType::Armor && type <= ItemType::Shield)
                return ItemCategory::Equipment;

            if (type >= ItemType::Food && type <= ItemType::Bomb)
                return ItemCategory::Consumables;

            return ItemCategory::Misc;
        }

        static ItemType FromString(const std::string& str)
        {
            auto value = magic_enum::enum_cast<ItemType>(str);

            if (value.has_value())
                return value.value();

            return ItemType::Other;
        }

        static std::string ToString(ItemType type)
        {
            return std::string(magic_enum::enum_name(type));
        }
    };
}