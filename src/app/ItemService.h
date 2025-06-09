#pragma once

#include "ItemData.h"

namespace app
{
    enum class ItemError : std::int8_t
    {
        NotFound = 0,
    };

    class ItemService
    {
    public:
        ItemService(const ItemDataMap& items);

        tl::expected<std::reference_wrapper<const ItemData>, ItemError>
            GetItemById(const std::string& id) const;

    private:
        const ItemDataMap& m_items;
    };
}
