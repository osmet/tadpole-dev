#pragma once

#include "Item.h"

namespace app_domain
{
    enum class ItemError : std::int8_t
    {
        NotFound = 0,
    };

    class ItemService
    {
    public:
        ItemService(const ItemMap& items);

        tl::expected<std::reference_wrapper<const Item>, ItemError>
            GetItemById(const std::string& id) const;

    private:
        const ItemMap& m_items;
    };
}
