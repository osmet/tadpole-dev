#include <catch.hpp>
#include <tl/expected.hpp>
#include "TradeService.h"
#include "CharacterService.h"
#include "InventoryService.h"
#include "ItemService.h"

using namespace app_domain;

TEST_CASE("TradeService::TradeItem_FewValidTrades_Succeeds")
{
    Item item1;
    item1.Id = "item_1";
    item1.Value = 200;

    Item item2;
    item2.Id = "item_2";
    item2.Value = 300;

    Item item3;
    item3.Id = "item_3";
    item3.Value = 100;

    ItemMap items{
        {item1.Id, item1},
        {item2.Id, item2},
        {item3.Id, item3}
    };
    ItemService itemService(items);

    Character buyer;
    buyer.Id = "buyer";
    buyer.Name = "Buyer";
    buyer.InventoryId = "inventory_buyer";

    Character seller;
    seller.Id = "seller";
    seller.Name = "Seller";
    seller.InventoryId = "inventory_seller";

    Character third;
    third.Id = "third";
    third.Name = "Third";
    third.InventoryId = "inventory_third";

    CharacterMap characters{
        {buyer.Id, buyer},
        {seller.Id, seller},
        {third.Id, third}
    };
    CharacterService characterService(characters);

    Inventory buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 1000;

    InventoryItem inventoryItem1;
    inventoryItem1.ItemId = "item_1";

    InventoryItem inventoryItem2;
    inventoryItem2.ItemId = "item_2";

    InventoryItem inventoryItem3;
    inventoryItem3.ItemId = "item_3";

    Inventory sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;
    sellerInventory.Items.push_back(inventoryItem1); // Index 0
    sellerInventory.Items.push_back(inventoryItem2); // Index 1

    Inventory thirdInventory;
    thirdInventory.Id = "inventory_third";
    thirdInventory.CurrentMoney = 200;
    thirdInventory.Items.push_back(inventoryItem3); // Index 0

    InventoryMap inventories{
        {buyerInventory.Id, buyerInventory},
        {sellerInventory.Id, sellerInventory},
        {thirdInventory.Id, thirdInventory}
    };
    InventoryService inventoryService(inventories, itemService);

    TradeService tradeService(itemService, characterService, inventoryService);

    {
        auto result = tradeService.TradeItem("buyer", "seller", 0);

        REQUIRE(result.has_value());
        REQUIRE(inventories["inventory_buyer"].CurrentMoney == 800);
        REQUIRE(inventories["inventory_seller"].CurrentMoney == 700);
        REQUIRE(inventories["inventory_buyer"].Items.size() == 1);
        REQUIRE(inventories["inventory_buyer"].Items[0].ItemId == "item_1");
        REQUIRE(inventories["inventory_seller"].Items.size() == 1); // item_2 left
    }

    {
        auto result = tradeService.TradeItem("buyer", "seller", 0); // Index 0 again, since item_2 moved up

        REQUIRE(result.has_value());
        REQUIRE(inventories["inventory_buyer"].CurrentMoney == 500);
        REQUIRE(inventories["inventory_seller"].CurrentMoney == 1000);
        REQUIRE(inventories["inventory_buyer"].Items.size() == 2);
        REQUIRE(inventories["inventory_buyer"].Items[1].ItemId == "item_2");
        REQUIRE(inventories["inventory_seller"].Items.empty());
    }

    {
        auto result = tradeService.TradeItem("buyer", "third", 0);

        REQUIRE(result.has_value());
        REQUIRE(inventories["inventory_buyer"].CurrentMoney == 400);
        REQUIRE(inventories["inventory_third"].CurrentMoney == 300);
        REQUIRE(inventories["inventory_buyer"].Items.size() == 3);
        REQUIRE(inventories["inventory_buyer"].Items[2].ItemId == "item_3");
        REQUIRE(inventories["inventory_third"].Items.empty());
    }

    {
        const auto& buyerInv = inventories["inventory_buyer"];

        REQUIRE(buyerInv.CurrentMoney == 400);
        REQUIRE(buyerInv.Items.size() == 3);
        REQUIRE(buyerInv.Items[0].ItemId == "item_1");
        REQUIRE(buyerInv.Items[1].ItemId == "item_2");
        REQUIRE(buyerInv.Items[2].ItemId == "item_3");
    }
}

TEST_CASE("TradeService::TradeItem_BuyerNotFound_ReturnsCharacterNotFound")
{
    ItemMap items;
    ItemService itemService(items);

    CharacterMap characters; // Empty
    CharacterService characterService(characters);

    InventoryMap inventories;
    InventoryService inventoryService(inventories, itemService);

    TradeService tradeService(itemService, characterService, inventoryService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::CharacterNotFound);
}

TEST_CASE("TradeService::TradeItem_InventoryNotFound_ReturnsInventoryNotFound")
{
    ItemMap items;
    ItemService itemService(items);

    Character buyer;
    buyer.Id = "buyer";
    buyer.InventoryId = "inventory_buyer";

    Character seller;
    seller.Id = "seller";
    seller.InventoryId = "inventory_seller";

    CharacterMap characters{
        {"buyer", buyer},
        {"seller", seller}
    };
    CharacterService characterService(characters);

    InventoryMap inventories; // Missing inventories
    InventoryService inventoryService(inventories, itemService);

    TradeService tradeService(itemService, characterService, inventoryService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::InventoryNotFound);
}

TEST_CASE("TradeService::TradeItem_ItemIndexInvalid_ReturnsInventoryItemNotFound")
{
    ItemMap items;
    ItemService itemService(items);

    Character buyer;
    buyer.Id = "buyer";
    buyer.InventoryId = "inventory_buyer";

    Character seller;
    seller.Id = "seller";
    seller.InventoryId = "inventory_seller";

    CharacterMap characters{
        {"buyer", buyer},
        {"seller", seller}
    };
    CharacterService characterService(characters);

    Inventory buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 1000;

    Inventory sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;

    InventoryMap inventories{
        {buyerInventory.Id, buyerInventory},
        {sellerInventory.Id, sellerInventory}
    };
    InventoryService inventoryService(inventories, itemService);

    TradeService tradeService(itemService, characterService, inventoryService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::InventoryItemNotFound);
}

TEST_CASE("TradeService::TradeItem_MissingItem_ReturnsItemNotFound")
{
    ItemMap items; // Item is missing
    ItemService itemService(items);

    Character buyer;
    buyer.Id = "buyer";
    buyer.InventoryId = "inventory_buyer";

    Character seller;
    seller.Id = "seller";
    seller.InventoryId = "inventory_seller";

    CharacterMap characters{
        {"buyer", buyer},
        {"seller", seller}
    };
    CharacterService characterService(characters);

    InventoryItem item;
    item.ItemId = "missing_item";

    Inventory buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 1000;

    Inventory sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;
    sellerInventory.Items.push_back(item);

    InventoryMap inventories{
        {buyerInventory.Id, buyerInventory},
        {sellerInventory.Id, sellerInventory}
    };
    InventoryService inventoryService(inventories, itemService);

    TradeService tradeService(itemService, characterService, inventoryService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::ItemNotFound);
}

TEST_CASE("TradeService::TradeItem_NotEnoughMoney_ReturnsNotEnoughMoney")
{
    Item item;
    item.Id = "item_1";
    item.Value = 100;

    ItemMap items{
        {item.Id, item}
    };
    ItemService itemService(items);

    Character buyer;
    buyer.Id = "buyer";
    buyer.InventoryId = "inventory_buyer";

    Character seller;
    seller.Id = "seller";
    seller.InventoryId = "inventory_seller";

    CharacterMap characters{
        {"buyer", buyer},
        {"seller", seller}
    };
    CharacterService characterService(characters);

    InventoryItem inventoryItem1;
    inventoryItem1.ItemId = "item_1";

    Inventory buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 50; // Not enough

    Inventory sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;
    sellerInventory.Items.push_back(inventoryItem1);

    InventoryMap inventories{
        {buyerInventory.Id, buyerInventory},
        {sellerInventory.Id, sellerInventory}
    };
    InventoryService inventoryService(inventories, itemService);

    TradeService tradeService(itemService, characterService, inventoryService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::NotEnoughMoney);
}

TEST_CASE("TradeService::TradeItem_EmptyItemId_ReturnsItemNotFound")
{
    ItemMap items;
    ItemService itemService(items);

    Character buyer;
    buyer.Id = "buyer";
    buyer.Name = "Buyer";
    buyer.InventoryId = "inventory_buyer";

    Character seller;
    seller.Id = "seller";
    seller.Name = "Seller";
    seller.InventoryId = "inventory_seller";

    CharacterMap characters;
    characters[buyer.Id] = buyer;
    characters[seller.Id] = seller;
    CharacterService characterService(characters);

    InventoryItem invalidItem;
    invalidItem.ItemId = "";

    Inventory buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 1000;

    Inventory sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;
    sellerInventory.Items.push_back(invalidItem);

    InventoryMap inventories;
    inventories[buyerInventory.Id] = buyerInventory;
    inventories[sellerInventory.Id] = sellerInventory;

    InventoryService inventoryService(inventories, itemService);
    TradeService tradeService(itemService, characterService, inventoryService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);
    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::ItemNotFound);
}

TEST_CASE("TradeService::IsItemTradable_ReturnsCorrectValue")
{
    Item item1;
    item1.Id = "item_1";
    item1.Value = 100;
    item1.IsStoryItem = false; // Tradable

    Item item2;
    item2.Id = "item_2";
    item2.Value = 200;
    item2.IsStoryItem = true; // Not tradable

    ItemMap items{
        {item1.Id, item1},
        {item2.Id, item2}
    };
    ItemService itemService(items);

    InventoryItem inventoryItem1;
    inventoryItem1.ItemId = "item_1";

    InventoryItem inventoryItem2;
    inventoryItem2.ItemId = "item_2";

    Inventory inventory;
    inventory.Id = "inventory";
    inventory.Items.push_back(inventoryItem1);
    inventory.Items.push_back(inventoryItem2);

    InventoryMap inventories{
        {inventory.Id, inventory}
    };
    InventoryService inventoryService(inventories, itemService);

    CharacterMap characters;
    CharacterService characterService(characters);

    TradeService tradeService(itemService, characterService, inventoryService);

    auto itemDetails1 = inventoryService.GetItemDetails(inventory.Id, 0);
    auto itemDetails2 = inventoryService.GetItemDetails(inventory.Id, 1);

    REQUIRE(itemDetails1.has_value());
    REQUIRE(itemDetails2.has_value());

    REQUIRE(tradeService.IsItemTradable(itemDetails1.value().GetItem()));
    REQUIRE(!tradeService.IsItemTradable(itemDetails2.value().GetItem()));
}

TEST_CASE("TradeService::TradeItem_ItemNotTradable_ReturnsItemNotTradable")
{
    Item item;
    item.Id = "item_1";
    item.Value = 100;
    item.IsStoryItem = true;

    ItemMap items{
        {item.Id, item}
    };
    ItemService itemService(items);

    Character buyer;
    buyer.Id = "buyer";
    buyer.Name = "Buyer";
    buyer.InventoryId = "inventory_buyer";

    Character seller;
    seller.Id = "seller";
    seller.Name = "Seller";
    seller.InventoryId = "inventory_seller";

    CharacterMap characters;
    characters[buyer.Id] = buyer;
    characters[seller.Id] = seller;
    CharacterService characterService(characters);

    InventoryItem tradableItem;
    tradableItem.ItemId = "item_1";

    Inventory buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 1000;

    Inventory sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;
    sellerInventory.Items.push_back(tradableItem);

    InventoryMap inventories;
    inventories[buyerInventory.Id] = buyerInventory;
    inventories[sellerInventory.Id] = sellerInventory;

    InventoryService inventoryService(inventories, itemService);
    TradeService tradeService(itemService, characterService, inventoryService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);
    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::ItemNotTradable);
}

TEST_CASE("TradeService::TradeItem_SameCharacter_ReturnsTradeWithSelfNotAllowed")
{
    Item item;
    item.Id = "item_1";
    item.Value = 100;

    ItemMap items;
    items[item.Id] = item;
    ItemService itemService(items);

    Character character;
    character.Id = "buyer";
    character.Name = "Buyer";
    character.InventoryId = "inventory_buyer";

    CharacterMap characters;
    characters[character.Id] = character;
    CharacterService characterService(characters);

    InventoryItem inventoryItem;
    inventoryItem.ItemId = "item_1";

    Inventory inventory;
    inventory.Id = "inventory_buyer";
    inventory.CurrentMoney = 1000;
    inventory.Items.push_back(inventoryItem);

    InventoryMap inventories;
    inventories[inventory.Id] = inventory;

    InventoryService inventoryService(inventories, itemService);
    TradeService tradeService(itemService, characterService, inventoryService);

    auto result = tradeService.TradeItem("buyer", "buyer", 0);
    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::TradeWithSelfNotAllowed);
}

TEST_CASE("TradeService::CanTradeItem_InvalidIndex_ReturnsError")
{
    Item item;
    item.Id = "item_1";
    item.Value = 100;

    ItemMap items;
    items[item.Id] = item;
    ItemService itemService(items);

    Character buyer;
    buyer.Id = "buyer";
    buyer.Name = "Buyer";
    buyer.InventoryId = "inventory_buyer";

    Character seller;
    seller.Id = "seller";
    seller.Name = "Seller";
    seller.InventoryId = "inventory_seller";

    CharacterMap characters;
    characters[buyer.Id] = buyer;
    characters[seller.Id] = seller;
    CharacterService characterService(characters);

    Inventory buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 1000;

    Inventory sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 100;

    InventoryMap inventories;
    inventories[buyerInventory.Id] = buyerInventory;
    inventories[sellerInventory.Id] = sellerInventory;

    InventoryService inventoryService(inventories, itemService);
    TradeService tradeService(itemService, characterService, inventoryService);

    auto result = tradeService.CanTradeItem("buyer", "seller", -1);
    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::InventoryItemNotFound);
}

class MockInventoryService : public InventoryService
{
public:
    bool failTransferMoney = false;
    bool failTransferItem = false;

    MockInventoryService(InventoryMap& inventories, IItemService& itemService)
        : InventoryService(inventories, itemService)
    {
    }

    tl::expected<void, InventoryError> TransferMoney(
        const std::string& fromInventoryId,
        const std::string& toInventoryId,
        std::uint32_t amount) override
    {
        if (failTransferMoney)
            return tl::unexpected(InventoryError::NotFound);

        return InventoryService::TransferMoney(fromInventoryId, toInventoryId, amount);
    }

    tl::expected<void, InventoryError> TransferItem(
        const std::string& fromInventoryId,
        const std::string& toInventoryId,
        std::size_t itemIndex) override
    {
        if (failTransferItem)
            return tl::unexpected(InventoryError::NotFound);

        return InventoryService::TransferItem(fromInventoryId, toInventoryId, itemIndex);
    }
};

TEST_CASE("TradeService::TradeItem_TransferFails_ReturnsTransferFailed")
{
    Item item;
    item.Id = "item_1";
    item.Value = 100;

    ItemMap items;
    items[item.Id] = item;
    ItemService itemService(items);

    Character buyer;
    buyer.Id = "buyer";
    buyer.Name = "Buyer";
    buyer.InventoryId = "inventory_buyer";

    Character seller;
    seller.Id = "seller";
    seller.Name = "Seller";
    seller.InventoryId = "inventory_seller";

    CharacterMap characters;
    characters[buyer.Id] = buyer;
    characters[seller.Id] = seller;
    CharacterService characterService(characters);

    InventoryItem tradableItem;
    tradableItem.ItemId = "item_1";

    Inventory buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 1000;

    Inventory sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;
    sellerInventory.Items.push_back(tradableItem);

    InventoryMap inventories;
    inventories[buyerInventory.Id] = buyerInventory;
    inventories[sellerInventory.Id] = sellerInventory;

    MockInventoryService mockInventoryService(inventories, itemService);
    TradeService tradeService(itemService, characterService, mockInventoryService);

    SECTION("TransferMoney fails")
    {
        mockInventoryService.failTransferMoney = true;
        mockInventoryService.failTransferItem = false;

        auto result = tradeService.TradeItem("buyer", "seller", 0);

        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == TradeError::TransferFailed);
    }

    SECTION("TransferItem fails")
    {
        mockInventoryService.failTransferMoney = false;
        mockInventoryService.failTransferItem = true;

        auto result = tradeService.TradeItem("buyer", "seller", 0);

        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == TradeError::TransferFailed);
    }

    SECTION("Both succeed")
    {
        mockInventoryService.failTransferMoney = false;
        mockInventoryService.failTransferItem = false;

        auto result = tradeService.TradeItem("buyer", "seller", 0);

        REQUIRE(result.has_value());
    }
}
