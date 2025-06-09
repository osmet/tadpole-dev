#pragma once

#include "InventoryData.h"
#include <tl/expected.hpp>
#include <functional>

namespace app
{
    enum class InventoryError : std::int8_t
    {
        NotFound = 0,
        NotEnoughMoney = 1,
        InvalidAmount = 2,
        ItemNotFound = 3,
    };

    class InventoryService
    {
    public:
        InventoryService(InventoryDataMap& inventories);

        tl::expected<std::reference_wrapper<const InventoryData>, InventoryError>
            GetInventoryById(const std::string& id) const;

        tl::expected<std::reference_wrapper<const InventoryItemData>, InventoryError>
            GetItemByIndex(const std::string& inventoryId, std::size_t index) const;

        tl::expected<void, InventoryError>
            TransferMoney(const std::string& fromId, const std::string& toId, int32_t amount);

        tl::expected<void, InventoryError>
            TransferItemByIndex(const std::string& fromId, const std::string& toId, std::size_t index);

    private:
        InventoryDataMap& m_inventories;
    };
}