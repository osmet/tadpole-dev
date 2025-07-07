#pragma once

#include "Item.h"

namespace app_domain
{
    enum class ItemError : std::int8_t
    {
        NotFound = 0,
    };

    class IItemService 
    {
    public:
        virtual ~IItemService() = 0;

        virtual lang::ExpectedConstRef<Item, ItemError>
            GetItemById(const std::string& id) const = 0;
    };

    class ItemService : public IItemService
    {
    public:
        ItemService(const ItemMap& items);

        lang::ExpectedConstRef<Item, ItemError>
            GetItemById(const std::string& id) const override;

    private:
        const ItemMap& m_items;
    };
}
