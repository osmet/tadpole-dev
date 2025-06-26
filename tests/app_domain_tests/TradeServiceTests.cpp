#include "TestContext.h"

using namespace app_domain;

TEST_CASE("TradeService::TradeItem_FewValidTrades_Succeeds")
{
    TestContext context;

    {
        auto result = context.TradeService.TradeItem("character_1", "character_2", 0);

        REQUIRE(result.has_value());
        REQUIRE(context.Inventories["inventory_1"].CurrentMoney == 800);
        REQUIRE(context.Inventories["inventory_2"].CurrentMoney == 700);
        REQUIRE(context.Inventories["inventory_1"].Items.size() == 1);
        REQUIRE(context.Inventories["inventory_1"].Items[0].ItemId == "sword");
        REQUIRE(context.Inventories["inventory_2"].Items.size() == 1);
    }

    {
        auto result = context.TradeService.TradeItem("character_1", "character_2", 0);

        REQUIRE(result.has_value());
        REQUIRE(context.Inventories["inventory_1"].CurrentMoney == 200);
        REQUIRE(context.Inventories["inventory_2"].CurrentMoney == 1300);
        REQUIRE(context.Inventories["inventory_1"].Items.size() == 2);
        REQUIRE(context.Inventories["inventory_1"].Items[1].ItemId == "shield");
        REQUIRE(context.Inventories["inventory_2"].Items.empty());
    }

    {
        auto result = context.TradeService.TradeItem("character_1", "character_3", 0);

        REQUIRE(result.has_value());
        REQUIRE(context.Inventories["inventory_1"].CurrentMoney == 100);
        REQUIRE(context.Inventories["inventory_3"].CurrentMoney == 500);
        REQUIRE(context.Inventories["inventory_1"].Items.size() == 3);
        REQUIRE(context.Inventories["inventory_1"].Items[2].ItemId == "tool");
        REQUIRE(context.Inventories["inventory_3"].Items.empty());
    }

    {
        const auto& buyerInventory = context.Inventories["inventory_1"];

        REQUIRE(buyerInventory.CurrentMoney == 100);
        REQUIRE(buyerInventory.Items.size() == 3);
        REQUIRE(buyerInventory.Items[0].ItemId == "sword");
        REQUIRE(buyerInventory.Items[1].ItemId == "shield");
        REQUIRE(buyerInventory.Items[2].ItemId == "tool");
    }
}

TEST_CASE("TradeService::TradeItem_BuyerNotFound_ReturnsCharacterNotFound")
{
    TestContext context;

    context.Characters.clear();

    auto result = context.TradeService.TradeItem("character_1", "character_2", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == TradeError::CharacterNotFound);
}

TEST_CASE("TradeService::TradeItem_InventoryNotFound_ReturnsInventoryNotFound")
{
    TestContext context;

    context.Inventories.clear();

    auto result = context.TradeService.TradeItem("character_1", "character_2", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == TradeError::InventoryNotFound);
}

TEST_CASE("TradeService::TradeItem_ItemIndexInvalid_ReturnsInventoryItemNotFound")
{
    TestContext context;

    context.Inventories["inventory_2"].Items.clear();

    auto result = context.TradeService.TradeItem("character_1", "character_2", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == TradeError::InventoryItemNotFound);
}

TEST_CASE("TradeService::TradeItem_MissingItem_ReturnsItemNotFound")
{
    TestContext context;

    context.Inventories["inventory_2"].Items.clear();
    context.Inventories["inventory_2"].Items.push_back({ "missing_item", 2 });

    auto result = context.TradeService.TradeItem("character_1", "character_2", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == TradeError::ItemNotFound);
}

TEST_CASE("TradeService::TradeItem_NotEnoughMoney_ReturnsNotEnoughMoney")
{
    TestContext context;

    context.Inventories["inventory_1"].CurrentMoney = 50;

    auto result = context.TradeService.TradeItem("character_1", "character_2", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == TradeError::NotEnoughMoney);
}

TEST_CASE("TradeService::TradeItem_EmptyItemId_ReturnsItemNotFound")
{
    TestContext context;

    context.Inventories["inventory_2"].Items.clear();
    context.Inventories["inventory_2"].Items.push_back({ "", 2 });

    auto result = context.TradeService.TradeItem("character_1", "character_2", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == TradeError::ItemNotFound);
}

TEST_CASE("TradeService::IsItemTradable_ReturnsCorrectValue")
{
    TestContext context;

    context.Items["sword"].IsStoryItem = false;
    context.Items["shield"].IsStoryItem = true;

    REQUIRE(context.TradeService.IsItemTradable(context.Items["sword"]));
    REQUIRE(!context.TradeService.IsItemTradable(context.Items["shield"]));
}

TEST_CASE("TradeService::TradeItem_ItemNotTradable_ReturnsItemNotTradable")
{
    TestContext context;

    context.Items["sword"].IsStoryItem = true;

    auto result = context.TradeService.TradeItem("character_1", "character_2", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == TradeError::ItemNotTradable);
}

TEST_CASE("TradeService::TradeItem_SameCharacter_ReturnsTradeWithSelfNotAllowed")
{
    TestContext context;

    auto result = context.TradeService.TradeItem("character_1", "character_1", 0);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == TradeError::TradeWithSelfNotAllowed);
}

TEST_CASE("TradeService::CanTradeItem_InvalidIndex_ReturnsInventoryItemNotFound")
{
    TestContext context;

    auto result = context.TradeService.CanTradeItem("character_1", "character_2", 5);

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == TradeError::InventoryItemNotFound);
}

namespace
{
    class MockInventoryService : public InventoryService
    {
    public:

        bool FailTransferMoney = false;
        bool FailTransferItem = false;

        using InventoryService::InventoryService;

        tl::expected<void, InventoryError> TransferMoney(
            const std::string& fromInventoryId,
            const std::string& toInventoryId,
            std::uint32_t amount) override
        {
            if (FailTransferMoney)
                return tl::unexpected(InventoryError::NotFound);

            return InventoryService::TransferMoney(fromInventoryId, toInventoryId, amount);
        }

        tl::expected<void, InventoryError> TransferItem(
            const std::string& fromInventoryId,
            const std::string& toInventoryId,
            std::size_t itemIndex) override
        {
            if (FailTransferItem)
                return tl::unexpected(InventoryError::NotFound);

            return InventoryService::TransferItem(fromInventoryId, toInventoryId, itemIndex);
        }
    };
}

TEST_CASE("TradeService::TradeItem_TransferFails_ReturnsTransferFailed")
{
    TestContext context;

    MockInventoryService mockInventoryService(context.Inventories, context.ItemService);
    TradeService tradeService(context.ItemService, context.CharacterService, mockInventoryService);

    SECTION("TransferMoney fails")
    {
        mockInventoryService.FailTransferMoney = true;
        mockInventoryService.FailTransferItem = false;

        auto result = tradeService.TradeItem("character_1", "character_2", 0);

        REQUIRE(!result.has_value());
        REQUIRE(result.error() == TradeError::TransferFailed);
    }

    SECTION("TransferItem fails")
    {
        mockInventoryService.FailTransferMoney = false;
        mockInventoryService.FailTransferItem = true;

        auto result = tradeService.TradeItem("character_1", "character_2", 0);

        REQUIRE(!result.has_value());
        REQUIRE(result.error() == TradeError::TransferFailed);
    }

    SECTION("Both succeed")
    {
        mockInventoryService.FailTransferMoney = false;
        mockInventoryService.FailTransferItem = false;

        auto result = tradeService.TradeItem("character_1", "character_2", 0);

        REQUIRE(result.has_value());
    }
}