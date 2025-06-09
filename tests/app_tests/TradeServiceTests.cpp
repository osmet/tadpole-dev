#include <catch.hpp>
#include <tl/expected.hpp>
#include "TradeService.h"
#include "CharacterService.h"
#include "InventoryService.h"
#include "ItemService.h"

using namespace app;

TEST_CASE("TradeService::TradeItem_FewValidTrades_Succeeds")
{
    CharacterData buyer;
    buyer.Id = "buyer";
    buyer.Name = "Buyer";
    buyer.InventoryId = "inventory_buyer";

    CharacterData seller;
    seller.Id = "seller";
    seller.Name = "Seller";
    seller.InventoryId = "inventory_seller";

    CharacterData third;
    third.Id = "third";
    third.Name = "Third";
    third.InventoryId = "inventory_third";

    CharacterDataMap characters{
        {buyer.Id, buyer},
        {seller.Id, seller},
        {third.Id, third}
    };
    CharacterService characterService(characters);

    InventoryData buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 1000;

    InventoryItemData item1;
    item1.ItemId = "item_1";

    InventoryItemData item2;
    item2.ItemId = "item_2";

    InventoryItemData item3;
    item3.ItemId = "item_3";

    InventoryData sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;
    sellerInventory.Items.push_back(item1); // index 0
    sellerInventory.Items.push_back(item2); // index 1

    InventoryData thirdInventory;
    thirdInventory.Id = "inventory_third";
    thirdInventory.CurrentMoney = 200;
    thirdInventory.Items.push_back(item3); // index 0

    InventoryDataMap inventories{
        {buyerInventory.Id, buyerInventory},
        {sellerInventory.Id, sellerInventory},
        {thirdInventory.Id, thirdInventory}
    };
    InventoryService inventoryService(inventories);

    ItemData itemData1;
    itemData1.Id = "item_1";
    itemData1.Price = 200;

    ItemData itemData2;
    itemData2.Id = "item_2";
    itemData2.Price = 300;

    ItemData itemData3;
    itemData3.Id = "item_3";
    itemData3.Price = 100;

    ItemDataMap items{
        {itemData1.Id, itemData1},
        {itemData2.Id, itemData2},
        {itemData3.Id, itemData3}
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
    CharacterDataMap characters; // Empty
    CharacterService characterService(characters);

    InventoryDataMap inventories;
    InventoryService inventoryService(inventories);

    ItemDataMap items;
    ItemService itemService(items);

    TradeService tradeService(inventoryService, characterService, itemService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::CharacterNotFound);
}

TEST_CASE("TradeService::TradeItem_InventoryNotFound_ReturnsInventoryNotFound")
{
    CharacterData buyer;
    buyer.Id = "buyer";
    buyer.InventoryId = "inventory_buyer";

    CharacterData seller;
    seller.Id = "seller";
    seller.InventoryId = "inventory_seller";

    CharacterDataMap characters{
        {"buyer", buyer},
        {"seller", seller}
    };
    CharacterService characterService(characters);

    InventoryDataMap inventories; // missing inventories
    InventoryService inventoryService(inventories);

    ItemDataMap items;
    ItemService itemService(items);

    TradeService tradeService(inventoryService, characterService, itemService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::InventoryNotFound);
}

TEST_CASE("TradeService::TradeItem_ItemIndexInvalid_ReturnsInventoryItemNotFound")
{
    CharacterData buyer;
    buyer.Id = "buyer";
    buyer.InventoryId = "inventory_buyer";

    CharacterData seller;
    seller.Id = "seller";
    seller.InventoryId = "inventory_seller";

    CharacterDataMap characters{
        {"buyer", buyer},
        {"seller", seller}
    };
    CharacterService characterService(characters);

    InventoryData buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 1000;

    InventoryData sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;

    InventoryDataMap inventories{
        {buyerInventory.Id, buyerInventory},
        {sellerInventory.Id, sellerInventory}
    };
    InventoryService inventoryService(inventories);

    ItemDataMap items;
    ItemService itemService(items);

    TradeService tradeService(inventoryService, characterService, itemService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::InventoryItemNotFound);
}

TEST_CASE("TradeService::TradeItem_ItemNotFound_ReturnsItemNotFound")
{
    CharacterData buyer;
    buyer.Id = "buyer";
    buyer.InventoryId = "inventory_buyer";

    CharacterData seller;
    seller.Id = "seller";
    seller.InventoryId = "inventory_seller";

    CharacterDataMap characters{
        {"buyer", buyer},
        {"seller", seller}
    };
    CharacterService characterService(characters);

    InventoryItemData item;
    item.ItemId = "missing_item";

    InventoryData buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 1000;

    InventoryData sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;
    sellerInventory.Items.push_back(item);

    InventoryDataMap inventories{
        {buyerInventory.Id, buyerInventory},
        {sellerInventory.Id, sellerInventory}
    };
    InventoryService inventoryService(inventories);

    ItemDataMap items; // item is missing
    ItemService itemService(items);

    TradeService tradeService(inventoryService, characterService, itemService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::ItemNotFound);
}

TEST_CASE("TradeService::TradeItem_NotEnoughMoney_ReturnsNotEnoughMoney")
{
    CharacterData buyer;
    buyer.Id = "buyer";
    buyer.InventoryId = "inventory_buyer";

    CharacterData seller;
    seller.Id = "seller";
    seller.InventoryId = "inventory_seller";

    CharacterDataMap characters{
        {"buyer", buyer},
        {"seller", seller}
    };
    CharacterService characterService(characters);

    InventoryItemData item;
    item.ItemId = "item_1";

    InventoryData buyerInventory;
    buyerInventory.Id = "inventory_buyer";
    buyerInventory.CurrentMoney = 50; // not enough

    InventoryData sellerInventory;
    sellerInventory.Id = "inventory_seller";
    sellerInventory.CurrentMoney = 500;
    sellerInventory.Items.push_back(item);

    InventoryDataMap inventories{
        {buyerInventory.Id, buyerInventory},
        {sellerInventory.Id, sellerInventory}
    };
    InventoryService inventoryService(inventories);

    ItemData itemData;
    itemData.Id = "item_1";
    itemData.Price = 100;

    ItemDataMap items{
        {itemData.Id, itemData}
    };
    ItemService itemService(items);

    TradeService tradeService(inventoryService, characterService, itemService);

    auto result = tradeService.TradeItem("buyer", "seller", 0);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == TradeError::NotEnoughMoney);
}
