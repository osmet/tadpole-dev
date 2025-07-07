#pragma once

#include "Inventory.h"
#include <tl/expected.hpp>
#include <functional>
#include "ItemService.h"
#include "InventoryItemDetails.h"
#include "ItemSortMode.h"

namespace app_domain
{
    enum class InventoryError : std::int8_t
    {
        NotFound = 0,
        NotEnoughMoney = 1,
        InvalidAmount = 2,
        IndexOutOfRange = 3,
        ItemNotFound = 4
    };

    class InventoryService
    {
    public:
        struct ItemDetailsListResult
        {
            std::vector<InventoryItemDetails> Items;
            std::vector<std::size_t> FailedIndices;
        };

        static constexpr std::uint32_t TransferAll = 0u;

        InventoryService(InventoryMap& inventories, IItemService& itemService);

        lang::ExpectedConstRef<Inventory, InventoryError>
            GetInventoryById(const std::string& id) const;

        lang::ExpectedConstRef<InventoryItem, InventoryError>
            GetItem(const std::string& inventoryId, std::size_t itemIndex) const;

        lang::Expected<InventoryItemDetails, InventoryError>
            GetItemDetails(const std::string& inventoryId, std::size_t itemIndex) const;

        lang::Expected<ItemDetailsListResult, InventoryError>
            GetFilterSortItemDetailsList(const std::string& inventoryId,
                ItemCategory itemFilterCategory, ItemSortMode itemSortMode,
                std::function<bool(const InventoryItemDetails&)> customFilter = nullptr) const;

        virtual lang::Expected<void, InventoryError>
            TransferMoney(const std::string& fromId, const std::string& toId, 
                std::uint32_t amount);

        virtual lang::Expected<void, InventoryError>
            TransferItem(const std::string& fromId, const std::string& toId, 
                std::size_t itemIndex, std::uint32_t count = InventoryService::TransferAll);

        lang::Expected<float, InventoryError> 
            CalculateCurrentWeight(const std::string& inventoryId) const;

        lang::Expected<void, InventoryError>
            CanStackItem(const std::string& inventoryId, std::size_t fromItemIndex,
                std::size_t toItemIndex, std::uint32_t count = InventoryService::TransferAll);

        lang::Expected<void, InventoryError>
            StackItem(const std::string& inventoryId, std::size_t fromItemIndex, 
                std::size_t toItemIndex, std::uint32_t count = InventoryService::TransferAll);

    private:
        struct CanStackItemResult
        {
            Inventory& Inventory;
            InventoryItem& FromItem;
            InventoryItem& ToItem;
            std::uint32_t CountToStack;
        };

        void AddItemInternal(Inventory& inventory, const std::string& itemId, std::uint32_t count);

        lang::Expected<CanStackItemResult, InventoryError>
            CanStackItemInternal(const std::string& inventoryId, std::size_t fromItemIndex,
                std::size_t toItemIndex, std::uint32_t count = InventoryService::TransferAll);

        InventoryMap& m_inventories;

        IItemService& m_itemService;
    };
}