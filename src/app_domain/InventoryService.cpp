#include "Precompiled.h"
#include "InventoryService.h"
#include "TimeProvider.h"

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
        InventoryService::GetItem(const std::string& inventoryId, std::size_t itemIndex) const
    {
        auto it = m_inventories.find(inventoryId);
        if (it == m_inventories.end())
            return tl::unexpected(InventoryError::NotFound);

        const auto& inventory = it->second;
        if (itemIndex >= inventory.Items.size())
            return tl::unexpected(InventoryError::IndexOutOfRange);

        return std::cref(inventory.Items[itemIndex]);
    }

    tl::expected<InventoryItemDetails, InventoryError>
        InventoryService::GetItemDetails(const std::string& inventoryId, std::size_t itemIndex) const
    {
        auto it = m_inventories.find(inventoryId);
        if (it == m_inventories.end())
            return tl::unexpected(InventoryError::NotFound);

        const auto& inventory = it->second;
        if (itemIndex >= inventory.Items.size())
            return tl::unexpected(InventoryError::IndexOutOfRange);

        const auto& inventoryItem = inventory.Items[itemIndex];

        auto itemResult = m_itemService.GetItemById(inventoryItem.ItemId);
        if (!itemResult.has_value())
            return tl::unexpected(InventoryError::ItemNotFound);

        return InventoryItemDetails {
            itemIndex,
            std::cref(itemResult.value().get()),
            inventoryItem.Count,
            inventoryItem.ModifiedAt
        };
    }

    tl::expected<InventoryService::ItemDetailsListResult, InventoryError>
        InventoryService::GetFilterSortItemDetailsList(const std::string& inventoryId,
            ItemCategory itemFilterCategory, ItemSortMode itemSortMode) const
    {
        auto inventoryResult = GetInventoryById(inventoryId);
        if (!inventoryResult)
            return tl::unexpected(inventoryResult.error());

        const auto& inventory = inventoryResult.value().get();
        const auto& inventoryItems = inventoryResult->get().Items;

        ItemDetailsListResult result;
        result.Items.reserve(inventoryItems.size());

        for (size_t itemIndex = 0; itemIndex < inventory.Items.size(); ++itemIndex)
        {
            auto itemDetailsResult = GetItemDetails(inventoryId, itemIndex);
            if (!itemDetailsResult)
            {
                result.FailedIndices.push_back(itemIndex);
                continue;
            }
        
            auto& itemDetails = itemDetailsResult.value();

            if (itemFilterCategory == app_domain::ItemCategory::All 
                || app_domain::ItemTypeHelper::ToCategory(itemDetails.GetItem().Type) == itemFilterCategory)
                result.Items.emplace_back(std::move(itemDetails));
        }

        std::stable_sort(result.Items.begin(), result.Items.end(),
            [itemSortMode](const app_domain::InventoryItemDetails& a, const app_domain::InventoryItemDetails& b)
        {
            // Primary sort keys
            switch (itemSortMode)
            {
            case app_domain::ItemSortMode::Name:
                return a.GetItem().Name < b.GetItem().Name; // Asc

            case app_domain::ItemSortMode::Latest:
                if (a.GetModifiedAt() != b.GetModifiedAt())
                    return a.GetModifiedAt() > b.GetModifiedAt(); // Asc
                break;

            case app_domain::ItemSortMode::Type:
                if (a.GetItem().Type != b.GetItem().Type)
                    return a.GetItem().Type < b.GetItem().Type; // Asc
                break;

            case app_domain::ItemSortMode::Value:
                if (a.GetTotalValue() != b.GetTotalValue())
                    return a.GetTotalValue() > b.GetTotalValue(); // Desc
                break;

            case app_domain::ItemSortMode::Weight:
                if (a.GetTotalWeight() != b.GetTotalWeight())
                    return a.GetTotalWeight() > b.GetTotalWeight(); // Desc
                break;

            default:
                return false;
            }

            // Secondary sort key: Name (for grouping of identical items)
            return a.GetItem().Name < b.GetItem().Name; // Asc
        });


        return result;
    }

    tl::expected<void, InventoryError>
        InventoryService::TransferMoney(const std::string& fromId, const std::string& toId, std::uint32_t amount)
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
        InventoryService::TransferItem(const std::string& fromId, const std::string& toId, std::size_t itemIndex)
    {
        auto fromIt = m_inventories.find(fromId);
        auto toIt = m_inventories.find(toId);

        if (fromIt == m_inventories.end() || toIt == m_inventories.end())
            return tl::unexpected(InventoryError::NotFound);

        auto& fromInventory = fromIt->second;
        auto& toInventory = toIt->second;

        if (itemIndex >= fromInventory.Items.size())
            return tl::unexpected(InventoryError::IndexOutOfRange);

        const auto& inventoryItem = fromInventory.Items[itemIndex];
        AddItemInternal(toInventory, inventoryItem.ItemId, inventoryItem.Count);

        fromInventory.Items.erase(fromInventory.Items.begin() + itemIndex);

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

    void InventoryService::AddItemInternal(Inventory& inventory, const std::string& itemId, std::uint32_t count)
    {
        InventoryItem inventoryItem;
        inventoryItem.ItemId = itemId;
        inventoryItem.Count = count;
        inventoryItem.ModifiedAt = app_domain::TimeProvider::GetUnixTimeMilliseconds();

        inventory.Items.emplace_back(std::move(inventoryItem));
    }
}