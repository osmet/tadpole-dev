#include <catch.hpp>
#include <tl/expected.hpp>
#include "CharacterService.h"

using namespace app;

TEST_CASE("CharacterService::GetCharacterById_CharacterExists_ReturnsCharacter")
{
    CharacterData character1;
    character1.Id = "character_1";
    character1.Name = "Character1";
    character1.InventoryId = "inventory_1";

    CharacterData character2;
    character2.Id = "character_2";

    CharacterDataMap characters;
    characters["character_1"] = character1;
    characters["character_2"] = character2;

    CharacterService service(characters);

    auto result = service.GetCharacterById("character_1");

    REQUIRE(result.has_value());
    const auto& character = result.value().get();
    REQUIRE(character.Id == "character_1");
    REQUIRE(character.Name == "Character1");
    REQUIRE(character.InventoryId == "inventory_1");
}

TEST_CASE("CharacterService::GetCharacterById_CharacterDoesNotExist_ReturnsNotFoundError")
{
    CharacterData character1;
    character1.Id = "character_1";
    
    CharacterDataMap characters;
    characters["character_1"] = character1;

    CharacterService service(characters);

    auto result = service.GetCharacterById("character_2");

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == CharacterError::NotFound);
}
