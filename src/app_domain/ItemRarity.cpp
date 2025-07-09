#include "Precompiled.h"
#include "ItemRarity.h"
#include <magic_enum/magic_enum.hpp>

namespace app_domain
{
    ItemRarity ItemRarityHelper::FromString(const std::string& string)
    {
        auto valueOpt = magic_enum::enum_cast<ItemRarity>(string);
        if (valueOpt.has_value())
            return valueOpt.value();

        return ItemRarity::Common;
    }

    std::string ItemRarityHelper::ToString(ItemRarity type)
    {
        return std::string(magic_enum::enum_name(type));
    }
}