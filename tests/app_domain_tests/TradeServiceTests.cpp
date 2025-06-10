#include <catch.hpp>
#include <tl/expected.hpp>
#include "TradeService.h"
#include "CharacterService.h"
#include "InventoryService.h"
#include "ItemService.h"

using namespace app_domain;

TEST_CASE("TradeService::TradeItem_FewValidTrades_Succeeds")
{
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
    sellerInventory.Items.push_back(inventoryItem1); // index 0
    sellerInventory.Items.push_back(inventoryItem2); // index 1

    Inventory thirdInventory;
    thirdInventory.Id = "inventory_third";
    thirdInventory.CurrentMoney = 200;
    thirdInventory.Items.push_back(inventoryItem3); // index 0

    InventoryMap inventories{
        {buyerInventory.Id, buyerInventory},
        {sellerInventory.Id, sellerInventory},
        {thirdInventory.Id, thirdInventory}
    };
    InventoryService inventoryService(inventories);

    Item item1;
    item1.Id = "item_1";
    item1.Price = 200;

    Item item2;
    item2.Id = "item_2";
    item2.Price = 300;

    Item item3;
    item3.Id = "item_3";
    item3.Price = 100;

    ItemMap items{
        {item1.Id, item1},
        {item2.Id, item2},
        {item3.Id, item3}
    };
    ItemService itemService(items);

    TradeService tradeService(inventoryService, characterService, itemService);

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
        auto result = tradeService.TradeItem("buyer", "seller", 0); // index 0 again, since item_2 moved up

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
    CharacterMap characters; // Empty
    CharacterService characterService(characters);

    InventoryMap inventories;
    InventoryService inventoryService(inventories);

    ItemMap items;
    ItemService itemService(items);

    TradeService tradeService(inventoryService, characterService, itemService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::CharacterNotFound);
}

TEST_CASE("TradeService::TradeItem_InventoryNotFound_ReturnsInventoryNotFound")
{
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

    InventoryMap inventories; // missing inventories
    InventoryService inventoryService(inventories);

    ItemMap items;
    ItemService itemService(items);

    TradeService tradeService(inventoryService, characterService, itemService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::InventoryNotFound);
}

TEST_CASE("TradeService::TradeItem_ItemIndexInvalid_ReturnsInventoryItemNotFound")
{
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
    InventoryService inventoryService(inventories);

    ItemMap items;
    ItemService itemService(items);

    TradeService tradeService(inventoryService, characterService, itemService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::InventoryItemNotFound);
}

TEST_CASE("TradeService::TradeItem_ItemNotFound_ReturnsItemNotFound")
{
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
    InventoryService inventoryService(inventories);

    ItemMap items; // item is missing
    ItemService itemService(items);

    TradeService tradeService(inventoryService, characterService, itemService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::ItemNotFound);
}

TEST_CASE("TradeService::TradeItem_NotEnoughMoney_ReturnsNotEnoughMoney")
{
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
    buyerInventory.CurrentMoney = 50; // not enough

    Inventory sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;
    sellerInventory.Items.push_back(inventoryItem1);

    InventoryMap inventories{
        {buyerInventory.Id, buyerInventory},
        {sellerInventory.Id, sellerInventory}
    };
    InventoryService inventoryService(inventories);

    Item item;
    item.Id = "item_1";
    item.Price = 100;

    ItemMap items{
        {item.Id, item}
    };
    ItemService itemService(items);

    TradeService tradeService(inventoryService, characterService, itemService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::NotEnoughMoney);
}
