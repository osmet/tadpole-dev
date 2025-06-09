#include <catch.hpp>
#include <tl/expected.hpp>
#include "ItemService.h"

using namespace app;

TEST_CASE("ItemService::GetItemById_ItemExists_ReturnsItem")
{
    ItemData item;
    item.Id = "item_1";
    item.Name = "Sword";

    ItemDataMap items;
    items["item_1"] = item;

    ItemService service(items);

    auto result = service.GetItemById("item_1");

    REQUIRE(result.has_value());
    const auto& found = result.value().get();
    REQUIRE(found.Id == "item_1");
    REQUIRE(found.Name == "Sword");
}

TEST_CASE("ItemService::GetItemById_ItemDoesNotExist_ReturnsNotFoundError")
{
    ItemData item;
    item.Id = "item_1";

    ItemDataMap items;
    items["item_1"] = item;

    ItemService service(items);

    auto result = service.GetItemById("item_2");

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == ItemError::NotFound);
}