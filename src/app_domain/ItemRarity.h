#pragma once

#include <magic_enum/magic_enum.hpp>

namespace app_domain
{
    enum class ItemRarity : std::int8_t
    {
        Common = 0,
        Uncommon = 1,
        Rare = 2,
        VeryRare = 3,
        Legendary = 4
    };

    class ItemRarityHelper
    {
    public:
        static ItemRarity FromString(const std::string& str)
        {
            auto value = magic_enum::enum_cast<ItemRarity>(str);

            if (value.has_value())
                return value.value();

            return ItemRarity::Common;
        }

        static std::string ToString(ItemRarity type)
        {
            return std::string(magic_enum::enum_name(type));
        }
    };
}
