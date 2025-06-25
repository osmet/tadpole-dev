#pragma once

namespace app_domain
{
    struct Item;

    class InventoryItemDetails
    {
    public:
        InventoryItemDetails(std::size_t index, const Item& item, 
            std::uint32_t count, std::uint64_t modifiedAt);

        std::size_t GetIndex() const;
        const Item& GetItem() const;
        std::uint32_t GetCount() const;
        std::uint64_t GetModifiedAt() const;

        float GetTotalWeight() const;
        std::uint32_t GetTotalValue() const;

    private:
        std::size_t m_index = 0u;
        std::reference_wrapper<const Item> m_item;
        std::uint32_t m_count = 0u;
        std::uint64_t m_modifiedAt = 0u;
    };
}