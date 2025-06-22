#include "Precompiled.h"
#include "InventoryService.h"

namespace app_domain
{
    InventoryService::InventoryService(InventoryMap& inventories, IItemService& itemService)
        : m_inventories(inventories)
        , m_itemService(itemService)
        
    {
    }

    tl::expected<std::reference_wrapper<const Inventory>, InventoryError>
        InventoryService::GetInventoryById(const std::string& id) const
    {
        auto it = m_inventories.find(id);
        if (it == m_inventories.end())
            return tl::unexpected(InventoryError::NotFound);

        return std::cref(it->second);
    }

    tl::expected<std::reference_wrapper<const InventoryItem>, InventoryError>
        InventoryService::GetItemByIndex(const std::string& inventoryId, std::size_t index) const
    {
        auto it = m_inventories.find(inventoryId);
        if (it == m_inventories.end())
            return tl::unexpected(InventoryError::NotFound);

        const auto& inventory = it->second;
        if (index >= inventory.Items.size())
            return tl::unexpected(InventoryError::ItemNotFound);

        return std::cref(inventory.Items[index]);
    }

    tl::expected<void, InventoryError>
        InventoryService::TransferMoney(const std::string& fromId, const std::string& toId, int32_t amount)
    {
        if (amount <= 0)
            return tl::unexpected(InventoryError::InvalidAmount);

        auto fromIt = m_inventories.find(fromId);
        auto toIt = m_inventories.find(toId);

        if (fromIt == m_inventories.end() || toIt == m_inventories.end())
            return tl::unexpected(InventoryError::NotFound);

        if (fromIt->second.CurrentMoney < amount)
            return tl::unexpected(InventoryError::NotEnoughMoney);

        fromIt->second.CurrentMoney -= amount;
        toIt->second.CurrentMoney += amount;

        return {};
    }

    tl::expected<void, InventoryError>
        InventoryService::TransferItemByIndex(const std::string& fromId, const std::string& toId, std::size_t index)
    {
        auto fromIt = m_inventories.find(fromId);
        auto toIt = m_inventories.find(toId);

        if (fromIt == m_inventories.end() || toIt == m_inventories.end())
            return tl::unexpected(InventoryError::NotFound);

        auto& fromInventory = fromIt->second;
        auto& toInventory = toIt->second;

        if (index >= fromInventory.Items.size())
            return tl::unexpected(InventoryError::ItemNotFound);

        toInventory.Items.push_back(std::move(fromInventory.Items[index]));
        fromInventory.Items.erase(fromInventory.Items.begin() + index);

        return {};
    }

    tl::expected<float, InventoryError> InventoryService::CalculateCurrentWeight(const std::string& inventoryId) const
    {
        auto it = m_inventories.find(inventoryId);
        if (it == m_inventories.end())
            return tl::unexpected(InventoryError::NotFound);

        const auto& inventory = it->second;

        float currentWeight = 0.0f;
        for (const auto& inventoryItem : inventory.Items)
        {
            auto itemResult = m_itemService.GetItemById(inventoryItem.ItemId);
            if (itemResult.has_value())
            {
                const auto& item = itemResult.value().get();
                currentWeight += item.Weight * static_cast<float>(inventoryItem.Count);
            }
        }

        return currentWeight;
    }
}