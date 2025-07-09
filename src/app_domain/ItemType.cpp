#include "Precompiled.h"
#include "ItemType.h"
#include <magic_enum/magic_enum.hpp>

namespace app_domain
{
    ItemType ItemTypeHelper::FromString(const std::string& string)
    {
        auto valueOpt = magic_enum::enum_cast<ItemType>(string);
        if (valueOpt.has_value())
            return valueOpt.value();

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

        if (type >= ItemType::Food && type <= ItemType::Arrow)
            return ItemCategory::Consumables;

        if (type >= ItemType::Scroll && type <= ItemType::Bomb)
            return ItemCategory::ScrollsAndTools;

        return ItemCategory::Misc;
    }
}