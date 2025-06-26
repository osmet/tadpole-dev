#include "TestContext.h"

using namespace app_domain;

TEST_CASE("CharacterService::GetCharacterById_CharacterExists_ReturnsCharacter")
{
    TestContext context;

    auto result = context.CharacterService.GetCharacterById("character_1");

    REQUIRE(result.has_value());
    const auto& character = result.value().get();
    REQUIRE(character.Id == "character_1");
    REQUIRE(character.Name == "Character 1");
    REQUIRE(character.InventoryId == "inventory_1");
}

TEST_CASE("CharacterService::GetCharacterById_CharacterDoesNotExist_ReturnsNotFound")
{
    TestContext context;

    context.Characters.clear();

    auto result = context.CharacterService.GetCharacterById("character_100");

    REQUIRE(!result.has_value());
    REQUIRE(result.error() == CharacterError::NotFound);
}
