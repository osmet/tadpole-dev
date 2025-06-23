#pragma once

#include "Inventory.h"
#include <tl/expected.hpp>
#include <functional>
#include "ItemService.h"
#include "InventoryItemDetails.h"

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
        InventoryService(InventoryMap& inventories, IItemService& itemService);

        tl::expected<std::reference_wrapper<const Inventory>, InventoryError>
            GetInventoryById(const std::string& id) const;

        tl::expected<std::reference_wrapper<const InventoryItem>, InventoryError>
            GetItem(const std::string& inventoryId, std::size_t itemIndex) const;

        tl::expected<InventoryItemDetails, InventoryError>
            GetItemDetails(const std::string& inventoryId, std::size_t itemIndex) const;

        virtual tl::expected<void, InventoryError>
            TransferMoney(const std::string& fromId, const std::string& toId, int32_t amount);

        virtual tl::expected<void, InventoryError>
            TransferItem(const std::string& fromId, const std::string& toId, std::size_t itemIndex);

        tl::expected<float, InventoryError> CalculateCurrentWeight(const std::string& inventoryId) const;

    private:
        InventoryMap& m_inventories;

        IItemService& m_itemService;
    };
}