#include "Precompiled.h"
#include "InventoryItemDetails.h"
#include "Item.h"

namespace app_domain
{
    InventoryItemDetails::InventoryItemDetails(std::size_t index, const Item& item, 
        std::uint32_t count, std::uint64_t modifiedAt)
        : m_index(index), m_item(item), m_count(count), m_modifiedAt(modifiedAt)
    {
    }

    std::size_t InventoryItemDetails::GetIndex() const
    {
        return m_index;
    }

    const Item& InventoryItemDetails::GetItem() const
    {
        return m_item.get();
    }

    std::uint32_t InventoryItemDetails::GetCount() const
    {
        return m_count;
    }

    std::uint64_t InventoryItemDetails::GetModifiedAt() const
    {
        return m_modifiedAt;
    }

    float InventoryItemDetails::GetTotalWeight() const
    {
        return GetItem().Weight * static_cast<float>(m_count);
    }

    std::uint32_t InventoryItemDetails::GetTotalValue() const
    {
        return GetItem().Value * m_count;
    }
}