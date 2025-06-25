#include "Precompiled.h"
#include "ItemType.h"
#include <magic_enum/magic_enum.hpp>

namespace app_domain
{
    ItemType ItemTypeHelper::FromString(const std::string& str)
    {
        auto value = magic_enum::enum_cast<ItemType>(str);

        if (value.has_value())
            return value.value();

        return ItemType::Misc;
    }

    std::string ItemTypeHelper::ToString(ItemType type)
    {
        return std::string(magic_enum::enum_name(type));
    }

    ItemCategory ItemTypeHelper::ToCategory(ItemType type)
    {
        if (type >= ItemType::Misc && type <= ItemType::Valuable)
            return ItemCategory::Misc;

        if (type >= ItemType::Book && type <= ItemType::Key)
            return ItemCategory::BooksAndKeys;

        if (type >= ItemType::Armor && type <= ItemType::Shield)
            return ItemCategory::Equipment;

        if (type >= ItemType::Food && type <= ItemType::Elixir)
            return ItemCategory::Consumables;

        if (type >= ItemType::Scroll && type <= ItemType::Bomb)
            return ItemCategory::ScrollsAndTools;

        return ItemCategory::Misc;
    }
}