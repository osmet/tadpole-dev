#include "Precompiled.h"
#include "ItemService.h"

namespace app
{
    ItemService::ItemService(const ItemDataMap& items)
        : m_items(items)
    {
    }

    tl::expected<std::reference_wrapper<const ItemData>, ItemError>
        ItemService::GetItemById(const std::string& id) const
    {
        auto it = m_items.find(id);
        if (it == m_items.end())
            return tl::unexpected(ItemError::NotFound);

        return std::cref(it->second);
    }
}
