﻿#include "Precompiled.h"
#include "InventoryService.h"
#include "../lang/Time.h"

namespace app_domain
{
    InventoryService::InventoryService(InventoryMap& inventories, IItemService& itemService)
        : m_inventories(inventories)
        , m_itemService(itemService)
        
    {
    }

    lang::ExpectedConstRef<Inventory, InventoryError>
        InventoryService::GetInventoryById(const std::string& id) const
    {
        auto it = m_inventories.find(id);
        if (it == m_inventories.end())
            return lang::Unexpected(InventoryError::NotFound);

        return std::cref(it->second);
    }

    lang::ExpectedConstRef<InventoryItem, InventoryError>
        InventoryService::GetItem(const std::string& inventoryId, std::size_t itemIndex) const
    {
        auto it = m_inventories.find(inventoryId);
        if (it == m_inventories.end())
            return lang::Unexpected(InventoryError::NotFound);

        const auto& inventory = it->second;
        if (itemIndex >= inventory.Items.size())
            return lang::Unexpected(InventoryError::IndexOutOfRange);

        return std::cref(inventory.Items[itemIndex]);
    }

    lang::Expected<InventoryItemDetails, InventoryError>
        InventoryService::GetItemDetails(const std::string& inventoryId, std::size_t itemIndex) const
    {
        auto it = m_inventories.find(inventoryId);
        if (it == m_inventories.end())
            return lang::Unexpected(InventoryError::NotFound);

        const auto& inventory = it->second;
        if (itemIndex >= inventory.Items.size())
            return lang::Unexpected(InventoryError::IndexOutOfRange);

        const auto& inventoryItem = inventory.Items[itemIndex];

        auto itemResult = m_itemService.GetItemById(inventoryItem.ItemId);
        if (!itemResult.has_value())
            return lang::Unexpected(InventoryError::ItemNotFound);

        return InventoryItemDetails {
            itemIndex,
            std::cref(itemResult.value().get()),
            inventoryItem.Count,
            inventoryItem.AddedAt
        };
    }

    lang::Expected<InventoryService::ItemDetailsListResult, InventoryError>
        InventoryService::GetFilterSortItemDetailsList(const std::string& inventoryId,
            ItemCategory itemFilterCategory, ItemSortMode itemSortMode,
            std::function<bool(const InventoryItemDetails&)> customFilter) const
    {
        auto inventoryResult = GetInventoryById(inventoryId);
        if (!inventoryResult.has_value())
            return lang::Unexpected(inventoryResult.error());

        const auto& inventory = inventoryResult.value().get();
        const auto& inventoryItems = inventoryResult->get().Items;

        ItemDetailsListResult result;
        result.Items.reserve(inventoryItems.size());

        for (size_t itemIndex = 0; itemIndex < inventory.Items.size(); ++itemIndex)
        {
            auto itemDetailsResult = GetItemDetails(inventoryId, itemIndex);
            if (!itemDetailsResult.has_value())
            {
                result.FailedIndices.push_back(itemIndex);
                continue;
            }
        
            auto& itemDetails = itemDetailsResult.value();

            bool categoryFilterPassed = itemFilterCategory == app_domain::ItemCategory::All
                || app_domain::ItemTypeHelper::ToCategory(itemDetails.GetItem().Type) == itemFilterCategory;

            bool customFilterPassed = !customFilter || customFilter(itemDetails);

            if (categoryFilterPassed && customFilterPassed)
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

    lang::Expected<void, InventoryError>
        InventoryService::TransferMoney(const std::string& fromId, const std::string& toId, 
            std::uint32_t amount)
    {
        if (amount <= 0u)
            return lang::Unexpected(InventoryError::InvalidAmount);

        auto fromIt = m_inventories.find(fromId);
        auto toIt = m_inventories.find(toId);

        if (fromIt == m_inventories.end() || toIt == m_inventories.end())
            return lang::Unexpected(InventoryError::NotFound);

        if (fromIt->second.CurrentMoney < amount)
            return lang::Unexpected(InventoryError::NotEnoughMoney);

        fromIt->second.CurrentMoney -= amount;
        toIt->second.CurrentMoney += amount;

        return {};
    }

    lang::Expected<void, InventoryError>
        InventoryService::TransferItem(const std::string& fromId, const std::string& toId, 
            std::size_t itemIndex, std::uint32_t count)
    {
        auto fromIt = m_inventories.find(fromId);
        auto toIt = m_inventories.find(toId);

        if (fromIt == m_inventories.end() || toIt == m_inventories.end())
            return lang::Unexpected(InventoryError::NotFound);

        auto& fromInventory = fromIt->second;
        auto& toInventory = toIt->second;

        if (itemIndex >= fromInventory.Items.size())
            return lang::Unexpected(InventoryError::IndexOutOfRange);

        auto& fromItem = fromInventory.Items[itemIndex];

        if (count == InventoryService::TransferAll)
            count = fromItem.Count;
        
        if (fromItem.Count < count)
            return lang::Unexpected(InventoryError::InvalidAmount);

        AddItemInternal(toInventory, fromItem.ItemId, count);

        fromItem.Count -= count;

        if (fromItem.Count == 0u)
            fromInventory.Items.erase(fromInventory.Items.begin() + itemIndex);

        return {};
    }

    lang::Expected<float, InventoryError> 
        InventoryService::CalculateCurrentWeight(const std::string& inventoryId) const
    {
        auto it = m_inventories.find(inventoryId);
        if (it == m_inventories.end())
            return lang::Unexpected(InventoryError::NotFound);

        const auto& inventory = it->second;

        float currentWeight = 0.0f;
        for (size_t itemIndex = 0; itemIndex < inventory.Items.size(); ++itemIndex)
        {
            auto itemDetailsResult = GetItemDetails(inventoryId, itemIndex);
            if (!itemDetailsResult.has_value())
                continue;

            auto& itemDetails = itemDetailsResult.value();

            currentWeight += itemDetails.GetTotalWeight();
        }

        return currentWeight;
    }

    lang::Expected<void, InventoryError>
        InventoryService::CanStackItem(const std::string& inventoryId, std::size_t fromItemIndex,
            std::size_t toItemIndex, std::uint32_t count)
    {
        auto result = CanStackItemInternal(inventoryId, fromItemIndex, toItemIndex, count);
        if (!result.has_value())
            return lang::Unexpected(result.error());

        return {};
    }

    lang::Expected<void, InventoryError>
        InventoryService::StackItem(const std::string& inventoryId, std::size_t fromItemIndex,
            std::size_t toItemIndex, std::uint32_t count)
    {
        auto canStackResult = CanStackItemInternal(inventoryId, fromItemIndex, toItemIndex, count);
        if (!canStackResult.has_value())
            return lang::Unexpected(canStackResult.error());

        auto& canStackValue = canStackResult.value();

        auto& inventory = canStackValue.Inventory;
        auto& fromItem = canStackValue.FromItem;
        auto& toItem = canStackValue.ToItem;
        auto countToStack = canStackValue.CountToStack;

        toItem.Count += countToStack;
        fromItem.Count -= countToStack;

        if (fromItem.Count == 0)
            inventory.Items.erase(inventory.Items.begin() + fromItemIndex);

        return {};
    }

    void InventoryService::AddItemInternal(Inventory& inventory, const std::string& itemId, std::uint32_t count)
    {
        InventoryItem inventoryItem;
        inventoryItem.ItemId = itemId;
        inventoryItem.Count = count;
        inventoryItem.AddedAt = lang::Time::GetUnixTimeMilliseconds();

        inventory.Items.emplace_back(std::move(inventoryItem));
    }

    lang::Expected<InventoryService::CanStackItemContext, InventoryError>
        InventoryService::CanStackItemInternal(const std::string& inventoryId, std::size_t fromItemIndex,
            std::size_t toItemIndex, std::uint32_t count)
    {
        if (fromItemIndex == toItemIndex)
            return lang::Unexpected(InventoryError::InvalidAmount);

        auto it = m_inventories.find(inventoryId);
        if (it == m_inventories.end())
            return lang::Unexpected(InventoryError::NotFound);

        auto& inventory = it->second;

        if (fromItemIndex >= inventory.Items.size() || toItemIndex >= inventory.Items.size())
            return lang::Unexpected(InventoryError::IndexOutOfRange);

        auto& fromItem = inventory.Items[fromItemIndex];
        auto& toItem = inventory.Items[toItemIndex];

        if (fromItem.ItemId != toItem.ItemId)
            return lang::Unexpected(InventoryError::ItemNotFound);

        if (count == InventoryService::TransferAll)
            count = fromItem.Count;

        if (fromItem.Count < count)
            return lang::Unexpected(InventoryError::InvalidAmount);

        return CanStackItemContext{
            .Inventory = inventory, 
            .FromItem = fromItem, 
            .ToItem = toItem, 
            .CountToStack = count 
        };
    }
}