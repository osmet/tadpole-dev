#include <catch.hpp>
#include <tl/expected.hpp>
#include "InventoryService.h"

using namespace app_domain;

class MockItemService : public app_domain::IItemService
{
public:
    MockItemService() = default;

    tl::expected<std::reference_wrapper<const app_domain::Item>, app_domain::ItemError>
        GetItemById(const std::string& id) const override
    {
        if (id == item1.Id) return item1;
        if (id == item2.Id) return item2;

        return tl::unexpected(app_domain::ItemError::NotFound);
    }

private:
    app_domain::Item item1{
        "item_1",
        "Sword",
        "Texture_UI_Item_Sword",
        "A sword.",
        app_domain::ItemType::Weapon,
        false,
        app_domain::ItemRarity::Uncommon,
        1.5f,
        100
    };

    app_domain::Item item2{
        "item_2",
        "Shield",
        "Texture_UI_Item_Shield",
        "A shield.",
        app_domain::ItemType::Armor,
        false,
        app_domain::ItemRarity::Rare,
        2.0f,
        150
    };
};

TEST_CASE("InventoryService::GetInventoryById_InventoryExists_ReturnsInventory")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.CurrentMoney = 100;

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetInventoryById("inventory_1");

    REQUIRE(result.has_value());
    REQUIRE(result.value().get().Id == "inventory_1");
    REQUIRE(result.value().get().CurrentMoney == 100);
}

TEST_CASE("InventoryService::GetInventoryById_InventoryDoesNotExist_ReturnsNotFoundError")
{
    InventoryMap inventories;
    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetInventoryById("inventory_1");

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetItem_ItemExists_ReturnsItem")
{
    InventoryItem item;
    item.ItemId = "item_1";
    item.Count = 1;

    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items.push_back(item);

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetItem("inventory_1", 0);

    REQUIRE(result.has_value());
    REQUIRE(result.value().get().ItemId == "item_1");
}

TEST_CASE("InventoryService::GetItem_InventoryDoesNotExist_ReturnsNotFoundError")
{
    InventoryMap inventories;
    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetItem("inventory_1", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetItem_IndexOutOfRange_ReturnsIndexOutOfRangeError")
{
    Inventory inventory;
    inventory.Id = "inventory_1";

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetItem("inventory_1", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::IndexOutOfRange);
}

TEST_CASE("InventoryService::TransferMoney_ValidTransfer_TransfersMoney")
{
    InventoryMap inventories;
    inventories["inventory_1"] = Inventory{ "inventory_1", 100, {} };
    inventories["inventory_2"] = Inventory{ "inventory_2", 50, {} };

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.TransferMoney("inventory_1", "inventory_2", 40);

    REQUIRE(result.has_value());
    REQUIRE(inventories["inventory_1"].CurrentMoney == 60);
    REQUIRE(inventories["inventory_2"].CurrentMoney == 90);
}

TEST_CASE("InventoryService::TransferMoney_ZeroAmount_ReturnsInvalidAmountError")
{
    InventoryMap inventories;
    inventories["inventory_1"] = Inventory{ "inventory_1", 100, {} };
    inventories["inventory_2"] = Inventory{ "inventory_2", 50, {} };

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.TransferMoney("inventory_1", "inventory_2", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::InvalidAmount);
}

TEST_CASE("InventoryService::TransferMoney_NotEnoughMoney_ReturnsNotEnoughMoneyError")
{
    InventoryMap inventories;
    inventories["inventory_1"] = Inventory{ "inventory_1", 10, {} };
    inventories["inventory_2"] = Inventory{ "inventory_2", 50, {} };

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.TransferMoney("inventory_1", "inventory_2", 20);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotEnoughMoney);
}

TEST_CASE("InventoryService::TransferMoney_InventoryNotFound_ReturnsNotFoundError")
{
    InventoryMap inventories;
    inventories["inventory_2"] = Inventory{ "inventory_2", 50, {} };

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.TransferMoney("inventory_1", "inventory_2", 10);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::TransferItem_ItemExists_TransfersItem")
{
    InventoryItem item{ "item_1", 2 };

    Inventory fromInventory{ "inventory_1", 0, {item} };
    Inventory toInventory{ "inventory_2", 0, {} };

    InventoryMap inventories;
    inventories["inventory_1"] = fromInventory;
    inventories["inventory_2"] = toInventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.TransferItem("inventory_1", "inventory_2", 0);

    REQUIRE(result.has_value());
    REQUIRE(inventories["inventory_1"].Items.empty());
    REQUIRE(inventories["inventory_2"].Items.size() == 1);
    REQUIRE(inventories["inventory_2"].Items[0].ItemId == "item_1");
}

TEST_CASE("InventoryService::TransferItem_IndexOutOfRange_ReturnsIndexOutOfRange")
{
    Inventory fromInventory{ "inventory_1", 0, {} };
    Inventory toInventory{ "inventory_2", 0, {} };

    InventoryMap inventories;
    inventories["inventory_1"] = fromInventory;
    inventories["inventory_2"] = toInventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.TransferItem("inventory_1", "inventory_2", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::IndexOutOfRange);
}

TEST_CASE("InventoryService::TransferItem_FromInventoryMissing_ReturnsNotFoundError")
{
    InventoryMap inventories;
    inventories["inventory_2"] = Inventory{ "inventory_2", 0, {} };

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.TransferItem("inventory_1", "inventory_2", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::TransferItem_ToInventoryMissing_ReturnsNotFoundError")
{
    InventoryItem item{ "item_1", 1 };
    Inventory fromInventory{ "inventory_1", 0, {item} };

    InventoryMap inventories;
    inventories["inventory_1"] = fromInventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.TransferItem("inventory_1", "inventory_2", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::CalculateCurrentWeight_CorrectWeightCalculation")
{
    InventoryMap inventories;
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
        { "item_1", 2 }, // 2 * 1.5 = 3.0
        { "item_2", 1 }  // 1 * 2.0 = 2.0
    };
    inventories[inventory.Id] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.CalculateCurrentWeight("inventory_1");

    REQUIRE(result.has_value());
    REQUIRE(result.value() == Approx(5.0f));
}

TEST_CASE("InventoryService::CalculateCurrentWeight_InventoryNotFound_ReturnsNotFoundError")
{
    InventoryMap inventories;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.CalculateCurrentWeight("nonexistent_inventory");

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetItemDetails_ItemExists_ReturnsDetails")
{
    InventoryItem item{ "item_1", 3 };

    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items.push_back(item);

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetItemDetails("inventory_1", 0);

    REQUIRE(result.has_value());
    REQUIRE(result->Index == 0);
    REQUIRE(result->Item.Id == "item_1");
    REQUIRE(result->Count == 3);
}

TEST_CASE("InventoryService::GetItemDetails_InventoryNotFound_ReturnsNotFoundError")
{
    InventoryMap inventories;
    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetItemDetails("inventory_1", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetItemDetails_IndexOutOfRange_ReturnsIndexOutOfRangeError")
{
    Inventory inventory;
    inventory.Id = "inventory_1";

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetItemDetails("inventory_1", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::IndexOutOfRange);
}

TEST_CASE("InventoryService::GetItemDetails_ItemNotFound_ReturnsItemNotFoundError")
{
    InventoryItem item{ "nonexistent_item", 1 };

    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items.push_back(item);

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetItemDetails("inventory_1", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::ItemNotFound);
}
