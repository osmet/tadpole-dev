#include "TestContext.h"

using namespace app_domain;

TEST_CASE("ItemService::GetItemById_ItemExists_ReturnsItem")
{
    TestContext context;

    auto result = context.ItemService.GetItemById("sword");

    REQUIRE(result.has_value());
    const auto& item = result.value().get();
    REQUIRE(item.Id == "sword");
    REQUIRE(item.Name == "Sword");
}

TEST_CASE("ItemService::GetItemById_ItemDoesNotExist_ReturnsNotFound")
{
    TestContext context;

    auto result = context.ItemService.GetItemById("item_100");

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == ItemError::NotFound);
}