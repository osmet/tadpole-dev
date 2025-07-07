#include "Precompiled.h"
#include "ItemService.h"

namespace app_domain
{
    IItemService::~IItemService() = default;

    ItemService::ItemService(const ItemMap& items)
        : m_items(items)
    {
    }

    lang::ExpectedConstRef<Item, ItemError>
        ItemService::GetItemById(const std::string& id) const
    {
        auto it = m_items.find(id);
        if (it == m_items.end())
            return tl::unexpected(ItemError::NotFound);

        return std::cref(it->second);
    }
}
