#include <catch.hpp>
#include <tl/expected.hpp>
#include "InventoryService.h"

using namespace app;

TEST_CASE("InventoryService::GetInventoryById_InventoryExists_ReturnsInventory")
{
    InventoryData inventory;
    inventory.Id = "inventory_1";
    inventory.CurrentMoney = 100;

    InventoryDataMap inventories;
    inventories["inventory_1"] = inventory;

    InventoryService service(inventories);

    auto result = service.GetInventoryById("inventory_1");

    REQUIRE(result.has_value());
    REQUIRE(result.value().get().Id == "inventory_1");
    REQUIRE(result.value().get().CurrentMoney == 100);
}

TEST_CASE("InventoryService::GetInventoryById_InventoryDoesNotExist_ReturnsNotFoundError")
{
    InventoryDataMap inventories;
    InventoryService service(inventories);

    auto result = service.GetInventoryById("inventory_1");

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetItemByIndex_ItemExists_ReturnsItem")
{
    InventoryItemData item;
    item.ItemId = "item_1";
    item.Count = 1;

    InventoryData inventory;
    inventory.Id = "inventory_1";
    inventory.Items.push_back(item);

    InventoryDataMap inventories;
    inventories["inventory_1"] = inventory;

    InventoryService service(inventories);

    auto result = service.GetItemByIndex("inventory_1", 0);

    REQUIRE(result.has_value());
    REQUIRE(result.value().get().ItemId == "item_1");
}

TEST_CASE("InventoryService::GetItemByIndex_InventoryDoesNotExist_ReturnsNotFoundError")
{
    InventoryDataMap inventories;
    InventoryService service(inventories);

    auto result = service.GetItemByIndex("inventory_1", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetItemByIndex_IndexOutOfBounds_ReturnsItemNotFoundError")
{
    InventoryData inventory;
    inventory.Id = "inventory_1";

    InventoryDataMap inventories;
    inventories["inventory_1"] = inventory;

    InventoryService service(inventories);

    auto result = service.GetItemByIndex("inventory_1", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::ItemNotFound);
}

TEST_CASE("InventoryService::TransferMoney_ValidTransfer_TransfersMoney")
{
    InventoryDataMap inventories;
    inventories["inventory_1"] = InventoryData{ "inventory_1", 100, {} };
    inventories["inventory_2"] = InventoryData{ "inventory_2", 50, {} };

    InventoryService service(inventories);

    auto result = service.TransferMoney("inventory_1", "inventory_2", 40);

    REQUIRE(result.has_value());
    REQUIRE(inventories["inventory_1"].CurrentMoney == 60);
    REQUIRE(inventories["inventory_2"].CurrentMoney == 90);
}

TEST_CASE("InventoryService::TransferMoney_ZeroAmount_ReturnsInvalidAmountError")
{
    InventoryDataMap inventories;
    inventories["inventory_1"] = InventoryData{ "inventory_1", 100, {} };
    inventories["inventory_2"] = InventoryData{ "inventory_2", 50, {} };

    InventoryService service(inventories);

    auto result = service.TransferMoney("inventory_1", "inventory_2", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::InvalidAmount);
}

TEST_CASE("InventoryService::TransferMoney_NotEnoughMoney_ReturnsNotEnoughMoneyError")
{
    InventoryDataMap inventories;
    inventories["inventory_1"] = InventoryData{ "inventory_1", 10, {} };
    inventories["inventory_2"] = InventoryData{ "inventory_2", 50, {} };

    InventoryService service(inventories);

    auto result = service.TransferMoney("inventory_1", "inventory_2", 20);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotEnoughMoney);
}

TEST_CASE("InventoryService::TransferMoney_InventoryNotFound_ReturnsNotFoundError")
{
    InventoryDataMap inventories;
    inventories["inventory_2"] = InventoryData{ "inventory_2", 50, {} };

    InventoryService service(inventories);

    auto result = service.TransferMoney("inventory_1", "inventory_2", 10);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::TransferItemByIndex_ItemExists_TransfersItem")
{
    InventoryItemData item{ "item_1", 2 };

    InventoryData fromInventory{ "inventory_1", 0, {item} };
    InventoryData toInventory{ "inventory_2", 0, {} };

    InventoryDataMap inventories;
    inventories["inventory_1"] = fromInventory;
    inventories["inventory_2"] = toInventory;

    InventoryService service(inventories);

    auto result = service.TransferItemByIndex("inventory_1", "inventory_2", 0);

    REQUIRE(result.has_value());
    REQUIRE(inventories["inventory_1"].Items.empty());
    REQUIRE(inventories["inventory_2"].Items.size() == 1);
    REQUIRE(inventories["inventory_2"].Items[0].ItemId == "item_1");
}

TEST_CASE("InventoryService::TransferItemByIndex_IndexOutOfRange_ReturnsItemNotFoundError")
{
    InventoryData fromInventory{ "inventory_1", 0, {} };
    InventoryData toInventory{ "inventory_2", 0, {} };

    InventoryDataMap inventories;
    inventories["inventory_1"] = fromInventory;
    inventories["inventory_2"] = toInventory;

    InventoryService service(inventories);

    auto result = service.TransferItemByIndex("inventory_1", "inventory_2", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::ItemNotFound);
}

TEST_CASE("InventoryService::TransferItemByIndex_FromInventoryMissing_ReturnsNotFoundError")
{
    InventoryDataMap inventories;
    inventories["inventory_2"] = InventoryData{ "inventory_2", 0, {} };

    InventoryService service(inventories);

    auto result = service.TransferItemByIndex("inventory_1", "inventory_2", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::TransferItemByIndex_ToInventoryMissing_ReturnsNotFoundError")
{
    InventoryItemData item{ "item_1", 1 };
    InventoryData fromInventory{ "inventory_1", 0, {item} };

    InventoryDataMap inventories;
    inventories["inventory_1"] = fromInventory;

    InventoryService service(inventories);

    auto result = service.TransferItemByIndex("inventory_1", "inventory_2", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}
