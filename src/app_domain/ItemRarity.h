#pragma once

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
        static ItemRarity FromString(const std::string& string);
        static std::string ToString(ItemRarity type);
    };
}
