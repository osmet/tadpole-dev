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
        if (id == item3.Id) return item3;
        if (id == item4.Id) return item4;
        if (id == item5.Id) return item5;
        if (id == item6.Id) return item6;
        if (id == item7.Id) return item7;
        if (id == item8.Id) return item8;
        if (id == item9.Id) return item9;

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

    app_domain::Item item3{
        "item_3",
        "Health Potion",
        "Texture_UI_Item_HealthPotion",
        "Restores health.",
        app_domain::ItemType::Potion,
        false,
        app_domain::ItemRarity::Common,
        0.5f,
        50
    };

    app_domain::Item item4{
        "item_4",
        "Diamond",
        "Texture_UI_Item_Diamond",
        "Valuable.",
        app_domain::ItemType::Misc,
        false,
        app_domain::ItemRarity::Common,
        0.1f,
        1
    };

    app_domain::Item item5{
        "item_5",
        "Key",
        "Texture_UI_Item_Key",
        "A key.",
        app_domain::ItemType::Key,
        false,
        app_domain::ItemRarity::Uncommon,
        0.2f,
        10
    };

    app_domain::Item item6{
        "item_6",
        "Book",
        "Texture_UI_Item_Book",
        "A book.",
        app_domain::ItemType::Book,
        false,
        app_domain::ItemRarity::Rare,
        1.0f,
        200
    };

    app_domain::Item item7{
        "item_7",
        "Scroll",
        "Texture_UI_Item_Scroll",
        "A scroll.",
        app_domain::ItemType::Scroll,
        false,
        app_domain::ItemRarity::VeryRare,
        0.3f,
        300
    };

    app_domain::Item item8{
        "item_8",
        "Tool",
        "Texture_UI_Item_Tool",
        "A tool.",
        app_domain::ItemType::Tool,
        false,
        app_domain::ItemRarity::Uncommon,
        0.4f,
        25
    };

    app_domain::Item item9{
        "item_9",
        "Mana Potion",
        "Texture_UI_Item_ManaPotion",
        "Restores health.",
        app_domain::ItemType::Potion,
        false,
        app_domain::ItemRarity::Common,
        0.5f,
        50
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
    REQUIRE(result->GetIndex() == 0);
    REQUIRE(result->GetItem().Id == "item_1");
    REQUIRE(result->GetCount() == 3);
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

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_InventoryNotFound_ReturnsNotFoundError")
{
    InventoryMap inventories;
    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("invalid_inventory", ItemCategory::All, ItemSortMode::Name);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_ItemNotFound_ReturnsNonEmptyFailedIndices")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
        { "item_1", 1 },
        { "invalid_id", 1 } // This will fail
    };

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("inventory_1", ItemCategory::All, ItemSortMode::Name);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 1);
    REQUIRE(result->Items[0].GetItem().Id == "item_1");
    REQUIRE(result->FailedIndices.size() == 1);
    REQUIRE(result->FailedIndices[0] == 1);
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_FilterByEquipment_ReturnsFilteredList")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
        { "item_1", 1 }, // Weapon
        { "item_2", 2 }, // Armor
        { "item_3", 3 }, // Potion
    };

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("inventory_1", ItemCategory::Equipment, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 2);
    REQUIRE(result->Items[0].GetItem().Id == "item_2");
    REQUIRE(result->Items[1].GetItem().Id == "item_1");
    REQUIRE(result->FailedIndices.empty());
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_FilterByConsumables_ReturnsFilteredList")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
        { "item_1", 1 }, // Weapon
        { "item_2", 2 }, // Armor
        { "item_3", 3 }, // Potion
    };

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("inventory_1", ItemCategory::Consumables, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 1);
    REQUIRE(result->Items[0].GetItem().Id == "item_3");
    REQUIRE(result->FailedIndices.empty());
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_FilterByMisc_ReturnsFilteredList")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
        { "item_1", 1 }, // Weapon
        { "item_2", 2 }, // Armor
        { "item_3", 3 }, // Potion
        { "item_4", 4 }, // Misc
    };

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("inventory_1", ItemCategory::Misc, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 1);
    REQUIRE(result->Items[0].GetItem().Id == "item_4");
    REQUIRE(result->FailedIndices.empty());
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_FilterByBooksAndKeys_ReturnsFilteredList")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
        { "item_1", 1 }, // Weapon
        { "item_2", 2 }, // Armor
        { "item_5", 5 }, // Key
        { "item_6", 6 }, // Book
    };

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("inventory_1", ItemCategory::BooksAndKeys, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 2);
    REQUIRE(result->Items[0].GetItem().Id == "item_6");
    REQUIRE(result->Items[1].GetItem().Id == "item_5");
    REQUIRE(result->FailedIndices.empty());
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_FilterByScrollsAndTools_ReturnsFilteredList")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
        { "item_1", 1 }, // Weapon
        { "item_7", 7 }, // Scroll
        { "item_8", 8 }, // Tool
    };

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("inventory_1", ItemCategory::ScrollsAndTools, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 2);
    REQUIRE(result->Items[0].GetItem().Id == "item_7");
    REQUIRE(result->Items[1].GetItem().Id == "item_8");
    REQUIRE(result->FailedIndices.empty());
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_SortByName_ReturnsSortedList")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
        { "item_3", 1 }, // Health Potion
        { "item_2", 1 }, // Armor
        { "item_9", 1 }, // Mana Potion
        { "item_1", 1 }, // Weapon
        { "item_3", 1 }  // Health Potion
    };

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("inventory_1", ItemCategory::All, ItemSortMode::Name);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 5);
    REQUIRE(result->Items[0].GetItem().Name == "Health Potion");
    REQUIRE(result->Items[1].GetItem().Name == "Health Potion");
    REQUIRE(result->Items[2].GetItem().Name == "Mana Potion");
    REQUIRE(result->Items[3].GetItem().Name == "Shield");
    REQUIRE(result->Items[4].GetItem().Name == "Sword");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_SortByLatest_ReturnsSortedList")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
        { "item_3", 1, 100 }, // Health Potion
        { "item_2", 1, 50 }, // Armor
        { "item_9", 1, 100 }, // Mana Potion
        { "item_1", 1, 50 }, // Weapon
        { "item_3", 1, 100 }  // Health Potion
    };

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("inventory_1", ItemCategory::All, ItemSortMode::Latest);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 5);
    REQUIRE(result->Items[0].GetItem().Id == "item_3");
    REQUIRE(result->Items[1].GetItem().Id == "item_3");
    REQUIRE(result->Items[2].GetItem().Id == "item_9");
    REQUIRE(result->Items[3].GetItem().Id == "item_2");
    REQUIRE(result->Items[4].GetItem().Id == "item_1");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_SortByType_ReturnsSortedList")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
        { "item_3", 1 }, // Health Potion
        { "item_2", 1 }, // Armor
        { "item_9", 1 }, // Mana Potion
        { "item_1", 1 }, // Weapon
        { "item_3", 1 }  // Health Potion
    };

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("inventory_1", ItemCategory::All, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 5);
    REQUIRE(result->Items[0].GetItem().Type == ItemType::Armor);
    REQUIRE(result->Items[1].GetItem().Type == ItemType::Weapon);
    REQUIRE(result->Items[2].GetItem().Type == ItemType::Potion);
    REQUIRE(result->Items[2].GetItem().Id == "item_3"); // Same Name
    REQUIRE(result->Items[3].GetItem().Type == ItemType::Potion);
    REQUIRE(result->Items[3].GetItem().Id == "item_3"); // Same Name
    REQUIRE(result->Items[4].GetItem().Type == ItemType::Potion);
    REQUIRE(result->Items[4].GetItem().Id == "item_9");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_SortByValue_ReturnsSortedList")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
         { "item_3", 1 }, // Health Potion
         { "item_2", 1 }, // Armor
         { "item_9", 1 }, // Mana Potion
         { "item_1", 1 }, // Weapon
         { "item_3", 1 }  // Health Potion
    };

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("inventory_1", ItemCategory::All, ItemSortMode::Value);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 5);
    REQUIRE(result->Items[0].GetItem().Id == "item_2");
    REQUIRE(result->Items[1].GetItem().Id == "item_1");
    REQUIRE(result->Items[2].GetItem().Id == "item_3");
    REQUIRE(result->Items[3].GetItem().Id == "item_3");
    REQUIRE(result->Items[4].GetItem().Id == "item_9");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_SortByWeight_ReturnsSortedList")
{
    Inventory inventory;
    inventory.Id = "inventory_1";
    inventory.Items = {
         { "item_3", 1 }, // Health Potion
         { "item_2", 1 }, // Armor
         { "item_9", 1 }, // Mana Potion
         { "item_1", 1 }, // Weapon
         { "item_3", 1 }  // Health Potion
    };

    InventoryMap inventories;
    inventories["inventory_1"] = inventory;

    MockItemService mockItemService;
    InventoryService service(inventories, mockItemService);

    auto result = service.GetFilterSortItemDetailsList("inventory_1", ItemCategory::All, ItemSortMode::Weight);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 5);
    REQUIRE(result->Items[0].GetItem().Id == "item_2");
    REQUIRE(result->Items[1].GetItem().Id == "item_1");
    REQUIRE(result->Items[2].GetItem().Id == "item_3");
    REQUIRE(result->Items[3].GetItem().Id == "item_3");
    REQUIRE(result->Items[4].GetItem().Id == "item_9");
}