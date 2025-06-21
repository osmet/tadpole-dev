#include "Precompiled.h"
#include "ItemRarity.h"
#include <magic_enum/magic_enum.hpp>

namespace app_domain
{
    ItemRarity ItemRarityHelper::FromString(const std::string& str)
    {
        auto value = magic_enum::enum_cast<ItemRarity>(str);

        if (value.has_value())
            return value.value();

        return ItemRarity::Common;
    }

    std::string ItemRarityHelper::ToString(ItemRarity type)
    {
        return std::string(magic_enum::enum_name(type));
    }
}