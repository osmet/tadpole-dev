#include <catch.hpp>
#include <tl/expected.hpp>
#include "ItemService.h"

using namespace app_domain;

TEST_CASE("ItemService::GetItemById_ItemExists_ReturnsItem")
{
    Item item;
    item.Id = "item_1";
    item.Name = "Sword";

    ItemMap items;
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
    Item item;
    item.Id = "item_1";

    ItemMap items;
    items["item_1"] = item;

    ItemService service(items);

    auto result = service.GetItemById("item_2");

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == ItemError::NotFound);
}