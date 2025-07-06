#include "TestContext.h"

using namespace app_domain;

TEST_CASE("InventoryService::GetInventoryById_InventoryExists_ReturnsInventory")
{
    TestContext context;

    auto result = context.InventoryService.GetInventoryById("inventory_1");

    REQUIRE(result.has_value());
    REQUIRE(result.value().get().Id == "inventory_1");
    REQUIRE(result.value().get().CurrentMoney == 1000);
}

TEST_CASE("InventoryService::GetInventoryById_InventoryNotFound_ReturnsNotFound")
{
    TestContext context;

    auto result = context.InventoryService.GetInventoryById("nonexistent_inventory");

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetItem_ItemExists_ReturnsItem")
{
    TestContext context;

    auto result = context.InventoryService.GetItem("inventory_2", 0);

    REQUIRE(result.has_value());
    REQUIRE(result.value().get().ItemId == "sword");
    REQUIRE(result.value().get().Count == 2);
}

TEST_CASE("InventoryService::GetItem_InventoryNotFound_ReturnsNotFound")
{
    TestContext context;

    auto result = context.InventoryService.GetItem("nonexistent_inventory", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetItem_IndexOutOfRange_ReturnsIndexOutOfRange")
{
    TestContext context;

    auto result = context.InventoryService.GetItem("inventory_2", 100);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::IndexOutOfRange);
}

TEST_CASE("InventoryService::TransferMoney_ValidTransfer_TransfersMoney")
{
    TestContext context;

    auto result = context.InventoryService.TransferMoney("inventory_1", "inventory_2", 100);

    REQUIRE(result.has_value());
    REQUIRE(context.Inventories["inventory_1"].CurrentMoney == 900);
    REQUIRE(context.Inventories["inventory_2"].CurrentMoney == 600);
}

TEST_CASE("InventoryService::TransferMoney_ZeroAmount_ReturnsInvalidAmount")
{
    TestContext context;

    auto result = context.InventoryService.TransferMoney("inventory_1", "inventory_2", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::InvalidAmount);
}


TEST_CASE("InventoryService::TransferMoney_NotEnoughMoney_ReturnsNotEnoughMoney")
{
    TestContext context;

    auto result = context.InventoryService.TransferMoney("inventory_1", "inventory_2", 2000);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::NotEnoughMoney);
}

TEST_CASE("InventoryService::TransferMoney_InventoryNotFound_ReturnsNotFound")
{
    TestContext context;

    auto result = context.InventoryService.TransferMoney("nonexistent_inventory", "inventory_2", 10);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::TransferItem_FewTransfersAll_Succeeds")
{
    TestContext context;

    {
        auto result = context.InventoryService.TransferItem("inventory_2", "inventory_1", 0);

        REQUIRE(result.has_value());
        REQUIRE(context.Inventories["inventory_2"].Items.size() == 1);
        REQUIRE(context.Inventories["inventory_1"].Items.size() == 1);
        REQUIRE(context.Inventories["inventory_1"].Items[0].ItemId == "sword");
        REQUIRE(context.Inventories["inventory_1"].Items[0].Count == 2);
    }

    {
        auto result = context.InventoryService.TransferItem("inventory_2", "inventory_1", 0);

        REQUIRE(result.has_value());
        REQUIRE(context.Inventories["inventory_2"].Items.size() == 0);
        REQUIRE(context.Inventories["inventory_1"].Items.size() == 2);
        REQUIRE(context.Inventories["inventory_1"].Items[1].ItemId == "shield");
        REQUIRE(context.Inventories["inventory_1"].Items[1].Count == 3);
    }

    {
        auto result = context.InventoryService.TransferItem("inventory_1", "inventory_2", 1);

        REQUIRE(result.has_value());
        REQUIRE(context.Inventories["inventory_2"].Items.size() == 1);
        REQUIRE(context.Inventories["inventory_2"].Items[0].ItemId == "shield");
        REQUIRE(context.Inventories["inventory_2"].Items[0].Count == 3);
        REQUIRE(context.Inventories["inventory_1"].Items.size() == 1);
    }
}

TEST_CASE("InventoryService::TransferItem_FewTransfersWithCount_Succeeds")
{
    TestContext context;

    {
        auto result = context.InventoryService.TransferItem("inventory_2", "inventory_1", 0, 1);

        REQUIRE(result.has_value());
        REQUIRE(context.Inventories["inventory_2"].Items.size() == 2);
        REQUIRE(context.Inventories["inventory_2"].Items[0].ItemId == "sword");
        REQUIRE(context.Inventories["inventory_2"].Items[0].Count == 1);
        REQUIRE(context.Inventories["inventory_1"].Items.size() == 1);
        REQUIRE(context.Inventories["inventory_1"].Items[0].ItemId == "sword");
        REQUIRE(context.Inventories["inventory_1"].Items[0].Count == 1);
    }

    {
        auto result = context.InventoryService.TransferItem("inventory_2", "inventory_1", 1, 2);

        REQUIRE(result.has_value());
        REQUIRE(context.Inventories["inventory_2"].Items.size() == 2);
        REQUIRE(context.Inventories["inventory_2"].Items[1].ItemId == "shield");
        REQUIRE(context.Inventories["inventory_2"].Items[1].Count == 1);
        REQUIRE(context.Inventories["inventory_1"].Items.size() == 2);
        REQUIRE(context.Inventories["inventory_1"].Items[1].ItemId == "shield");
        REQUIRE(context.Inventories["inventory_1"].Items[1].Count == 2);
    }

    {
        auto result = context.InventoryService.TransferItem("inventory_2", "inventory_1", 1, 1);

        REQUIRE(result.has_value());
        REQUIRE(context.Inventories["inventory_2"].Items.size() == 1);
        REQUIRE(context.Inventories["inventory_1"].Items.size() == 3);
        REQUIRE(context.Inventories["inventory_1"].Items[2].ItemId == "shield");
        REQUIRE(context.Inventories["inventory_1"].Items[2].Count == 1);
    }
}

TEST_CASE("InventoryService::TransferItem_CountMoreThanAvailable_ReturnsInvalidAmount")
{
    TestContext context;

    auto result = context.InventoryService.TransferItem("inventory_2", "inventory_1", 0, 100);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::InvalidAmount);
}

TEST_CASE("InventoryService::TransferItem_IndexOutOfRange_ReturnsIndexOutOfRange")
{
    TestContext context;

    auto result = context.InventoryService.TransferItem("inventory_2", "inventory_1", 5);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::IndexOutOfRange);
}

TEST_CASE("InventoryService::TransferItem_FromInventoryNotFound_ReturnsNotFound")
{
    TestContext context;

    auto result = context.InventoryService.TransferItem("nonexistent_inventory", "inventory_1", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::TransferItem_ToInventoryNotFound_ReturnsNotFound")
{
    TestContext context;

    auto result = context.InventoryService.TransferItem("inventory_2", "nonexistent_inventory", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::CalculateCurrentWeight_CorrectWeightCalculation")
{
    TestContext context;

    auto result = context.InventoryService.CalculateCurrentWeight("inventory_2");

    REQUIRE(result.has_value());
    REQUIRE(result.value() == Approx(12.f));
}

TEST_CASE("InventoryService::CalculateCurrentWeight_InventoryNotFound_ReturnsNotFound")
{
    TestContext context;

    auto result = context.InventoryService.CalculateCurrentWeight("nonexistent_inventory");

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetItemDetails_ItemExists_ReturnsDetails")
{
    TestContext context;

    auto result = context.InventoryService.GetItemDetails("inventory_2", 0);

    REQUIRE(result.has_value());
    REQUIRE(result->GetIndex() == 0);
    REQUIRE(result->GetItem().Id == "sword");
    REQUIRE(result->GetCount() == 2);
}

TEST_CASE("InventoryService::GetItemDetails_InventoryNotFound_ReturnsNotFound")
{
    TestContext context;

    auto result = context.InventoryService.GetItemDetails("nonexistent_inventory", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetItemDetails_IndexOutOfRange_ReturnsIndexOutOfRange")
{
    TestContext context;

    auto result = context.InventoryService.GetItemDetails("inventory_2", 100);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::IndexOutOfRange);
}

TEST_CASE("InventoryService::GetItemDetails_ItemNotFound_ReturnsItemNotFound")
{
    TestContext context;

    auto result = context.InventoryService.GetItemDetails("inventory_6", 1);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::ItemNotFound);
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_InventoryNotFound_ReturnsNotFound")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("nonexistent_inventory", ItemCategory::All, ItemSortMode::Name);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_ItemNotFound_ReturnsNonEmptyFailedIndices")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("inventory_6", ItemCategory::All, ItemSortMode::Name);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 3);
    REQUIRE(result->Items[0].GetItem().Id == "book");
    REQUIRE(result->Items[1].GetItem().Id == "diamond");
    REQUIRE(result->Items[2].GetItem().Id == "key");
    REQUIRE(result->FailedIndices.size() == 1);
    REQUIRE(result->FailedIndices[0] == 1);
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_FilterByEquipment_ReturnsFilteredList")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("inventory_4", ItemCategory::Equipment, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 2);
    REQUIRE(result->Items[0].GetItem().Id == "shield");
    REQUIRE(result->Items[1].GetItem().Id == "sword");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_FilterByConsumables_ReturnsFilteredList")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("inventory_4", ItemCategory::Consumables, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 3);
    REQUIRE(result->Items[0].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[1].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[2].GetItem().Id == "mana_potion");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_FilterByMisc_ReturnsFilteredList")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("inventory_6", ItemCategory::Misc, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 1);
    REQUIRE(result->Items[0].GetItem().Id == "diamond");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_FilterByBooksAndKeys_ReturnsFilteredList")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("inventory_6", ItemCategory::BooksAndKeys, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 2);
    REQUIRE(result->Items[0].GetItem().Id == "book");
    REQUIRE(result->Items[1].GetItem().Id == "key");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_FilterByScrollsAndTools_ReturnsFilteredList")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("inventory_5", ItemCategory::ScrollsAndTools, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 2);
    REQUIRE(result->Items[0].GetItem().Id == "scroll");
    REQUIRE(result->Items[1].GetItem().Id == "tool");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_CustomFilter_ReturnsFilteredList")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList(
        "inventory_4",
        app_domain::ItemCategory::All,
        app_domain::ItemSortMode::Name,
        [](const app_domain::InventoryItemDetails& details)
        {
            return details.GetItem().Value < 100u;
        });

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 3u);
    REQUIRE(result->Items[0].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[1].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[2].GetItem().Id == "mana_potion");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_SortByName_ReturnsSortedList")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("inventory_4", ItemCategory::All, ItemSortMode::Name);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 5);
    REQUIRE(result->Items[0].GetItem().Name == "Healing Potion");
    REQUIRE(result->Items[1].GetItem().Name == "Healing Potion");
    REQUIRE(result->Items[2].GetItem().Name == "Mana Potion");
    REQUIRE(result->Items[3].GetItem().Name == "Shield");
    REQUIRE(result->Items[4].GetItem().Name == "Sword");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_SortByLatest_ReturnsSortedList")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("inventory_4", ItemCategory::All, ItemSortMode::Latest);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 5);
    REQUIRE(result->Items[0].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[1].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[2].GetItem().Id == "mana_potion");
    REQUIRE(result->Items[3].GetItem().Id == "shield");
    REQUIRE(result->Items[4].GetItem().Id == "sword");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_SortByType_ReturnsSortedList")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("inventory_4", ItemCategory::All, ItemSortMode::Type);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 5);
    REQUIRE(result->Items[0].GetItem().Type == ItemType::Armor);
    REQUIRE(result->Items[1].GetItem().Type == ItemType::Weapon);
    REQUIRE(result->Items[2].GetItem().Type == ItemType::Potion);
    REQUIRE(result->Items[2].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[3].GetItem().Type == ItemType::Potion);
    REQUIRE(result->Items[3].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[4].GetItem().Type == ItemType::Potion);
    REQUIRE(result->Items[4].GetItem().Id == "mana_potion");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_SortByValue_ReturnsSortedList")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("inventory_4", ItemCategory::All, ItemSortMode::Value);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 5);
    REQUIRE(result->Items[0].GetItem().Id == "shield");
    REQUIRE(result->Items[1].GetItem().Id == "sword");
    REQUIRE(result->Items[2].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[3].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[4].GetItem().Id == "mana_potion");
}

TEST_CASE("InventoryService::GetFilterSortItemDetailsList_SortByWeight_ReturnsSortedList")
{
    TestContext context;

    auto result = context.InventoryService.GetFilterSortItemDetailsList("inventory_4", ItemCategory::All, ItemSortMode::Weight);

    REQUIRE(result.has_value());
    REQUIRE(result->Items.size() == 5);
    REQUIRE(result->Items[0].GetItem().Id == "shield");
    REQUIRE(result->Items[1].GetItem().Id == "sword");
    REQUIRE(result->Items[2].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[3].GetItem().Id == "healing_potion");
    REQUIRE(result->Items[4].GetItem().Id == "mana_potion");
}

TEST_CASE("InventoryService::CanStackItem_FromAndToIndicesEqual_ReturnsInvalidAmount")
{
    TestContext context;

    auto result = context.InventoryService.CanStackItem("inventory_stack", 0, 0, 1);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::InvalidAmount);
}

TEST_CASE("InventoryService::CanStackItem_InventoryNotFound_ReturnsNotFound")
{
    TestContext context;

    auto result = context.InventoryService.CanStackItem("nonexistent_inventory", 0, 1, 1);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::NotFound);
}

TEST_CASE("InventoryService::CanStackItem_FromIndexOutOfRange_ReturnsIndexOutOfRange")
{
    TestContext context;

    auto result = context.InventoryService.CanStackItem("inventory_stack", 100, 0, 1);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::IndexOutOfRange);
}

TEST_CASE("InventoryService::CanStackItem_ToIndexOutOfRange_ReturnsIndexOutOfRange")
{
    TestContext context;

    auto result = context.InventoryService.CanStackItem("inventory_stack", 0, 100, 1);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::IndexOutOfRange);
}

TEST_CASE("InventoryService::CanStackItem_ItemIdsDoNotMatch_ReturnsItemNotFound")
{
    TestContext context;

    auto result = context.InventoryService.CanStackItem("inventory_stack", 0, 1, 1);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::ItemNotFound);
}

TEST_CASE("InventoryService::CanStackItem_CountGreaterThanAvailable_ReturnsInvalidAmount")
{
    TestContext context;

    auto result = context.InventoryService.CanStackItem("inventory_stack", 0, 0, 1000);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::InvalidAmount);
}

TEST_CASE("InventoryService::CanStackItem_ValidInput_Succeeds")
{
    TestContext context;

    auto result = context.InventoryService.CanStackItem("inventory_stack", 0, 4, 1);

    REQUIRE(result.has_value());
}

TEST_CASE("InventoryService::StackItem_CanStackItemInternalFails_ReturnsSameError")
{
    TestContext context;

    auto result = context.InventoryService.StackItem("inventory_stack", 0, 0, 1);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == InventoryError::InvalidAmount);
}

TEST_CASE("InventoryService::StackItem_FewValidStacks_Succeeds")
{
    TestContext context;

    auto result = context.InventoryService.StackItem("inventory_stack", 4, 0, 3);

    REQUIRE(result.has_value());
    REQUIRE(context.Inventories["inventory_stack"].Items.size() == 5);
    REQUIRE(context.Inventories["inventory_stack"].Items[0].Count == 4);
    REQUIRE(context.Inventories["inventory_stack"].Items[4].Count == 2);

    result = context.InventoryService.StackItem("inventory_stack", 2, 0, InventoryService::TransferAll);

    REQUIRE(result.has_value());
    REQUIRE(context.Inventories["inventory_stack"].Items.size() == 4);
    REQUIRE(context.Inventories["inventory_stack"].Items[0].Count == 7);
}
