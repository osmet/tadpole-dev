#pragma once

#include <string>  
#include <unordered_map>
#include <vector>
#include <cstdint>

#include <tl/expected.hpp>
#include <catch.hpp>

#include "TradeService.h"
#include "CharacterService.h"
#include "InventoryService.h"
#include "ItemService.h"

struct TestContext
{
    app_domain::ItemMap Items{
        { "sword", app_domain::Item{
            .Id = "sword",
            .Name = "Sword",
            .IconTextureId = "Texture_UI_Item_Sword",
            .Description = "A sword.",
            .Type = app_domain::ItemType::Weapon,
            .IsStoryItem = false,
            .Rarity = app_domain::ItemRarity::Uncommon,
            .Weight = 1.5f,
            .Value = 100
        }},
        { "shield", app_domain::Item{
            .Id = "shield",
            .Name = "Shield",
            .IconTextureId = "Texture_UI_Item_Shield",
            .Description = "A shield.",
            .Type = app_domain::ItemType::Armor,
            .IsStoryItem = false,
            .Rarity = app_domain::ItemRarity::Rare,
            .Weight = 3.0f,
            .Value = 200
        }},
        { "healing_potion", app_domain::Item{
            .Id = "healing_potion",
            .Name = "Healing Potion",
            .IconTextureId = "Texture_UI_Item_Potion",
            .Description = "Restores health.",
            .Type = app_domain::ItemType::Potion,
            .IsStoryItem = false,
            .Rarity = app_domain::ItemRarity::Common,
            .Weight = 0.5f,
            .Value = 50
        }},
        { "diamond", app_domain::Item{
            .Id = "diamond",
            .Name = "Diamond",
            .IconTextureId = "Texture_UI_Item_Diamond",
            .Description = "Valuable.",
            .Type = app_domain::ItemType::Misc,
            .IsStoryItem = false,
            .Rarity = app_domain::ItemRarity::Common,
            .Weight = 0.1f,
            .Value = 1000
        }},
        { "key", app_domain::Item{
            .Id = "key",
            .Name = "Key",
            .IconTextureId = "Texture_UI_Item_Key",
            .Description = "A key.",
            .Type = app_domain::ItemType::Key,
            .IsStoryItem = false,
            .Rarity = app_domain::ItemRarity::Uncommon,
            .Weight = 0.2f,
            .Value = 10
        }},
        { "book", app_domain::Item{
            .Id = "book",
            .Name = "Book",
            .IconTextureId = "Texture_UI_Item_Book",
            .Description = "A book.",
            .Type = app_domain::ItemType::Book,
            .IsStoryItem = false,
            .Rarity = app_domain::ItemRarity::Rare,
            .Weight = 1.0f,
            .Value = 200
        }},
        { "scroll", app_domain::Item{
            .Id = "scroll",
            .Name = "Scroll",
            .IconTextureId = "Texture_UI_Item_Scroll",
            .Description = "A scroll.",
            .Type = app_domain::ItemType::Scroll,
            .IsStoryItem = false,
            .Rarity = app_domain::ItemRarity::VeryRare,
            .Weight = 0.3f,
            .Value = 300
        }},
        { "tool", app_domain::Item{
            .Id = "tool",
            .Name = "Tool",
            .IconTextureId = "Texture_UI_Item_Tool",
            .Description = "A tool.",
            .Type = app_domain::ItemType::Tool,
            .IsStoryItem = false,
            .Rarity = app_domain::ItemRarity::Uncommon,
            .Weight = 0.4f,
            .Value = 100
        }},
        { "mana_potion", app_domain::Item{
            .Id = "mana_potion",
            .Name = "Mana Potion",
            .IconTextureId = "Texture_UI_Item_ManaPotion",
            .Description = "Restores mana.",
            .Type = app_domain::ItemType::Potion,
            .IsStoryItem = false,
            .Rarity = app_domain::ItemRarity::Common,
            .Weight = 0.5f,
            .Value = 50
        }}
    };

    app_domain::CharacterMap Characters{
        { "character_1", app_domain::Character{
            .Id = "character_1",
            .Name = "Character 1",
            .PortraitTextureId = "",
            .MaxWeight = 100.f,
            .InventoryId = "inventory_1"
        }},
        { "character_2", app_domain::Character{
            .Id = "character_2",
            .Name = "Character 2",
            .PortraitTextureId = "",
            .MaxWeight = 100.f,
            .InventoryId = "inventory_2"
        }},
        { "character_3", app_domain::Character{
            .Id = "character_3",
            .Name = "Character 3",
            .PortraitTextureId = "",
            .MaxWeight = 100.f,
            .InventoryId = "inventory_3"
        }}
    };

    app_domain::InventoryMap Inventories{
        { "inventory_1", app_domain::Inventory{
            .Id = "inventory_1",
            .CurrentMoney = 1000,
            .Items = {}
        }},
        { "inventory_2", app_domain::Inventory{
            .Id = "inventory_2",
            .CurrentMoney = 500,
            .Items = {
                app_domain::InventoryItem{.ItemId = "sword", .Count = 2 },
                app_domain::InventoryItem{.ItemId = "shield", .Count = 3 }
            }
        }},
        { "inventory_3", app_domain::Inventory{
            .Id = "inventory_3",
            .CurrentMoney = 400,
            .Items = {
                app_domain::InventoryItem{.ItemId = "tool", .Count = 1 }
            }
        }},
        { "inventory_4", app_domain::Inventory{
            .Id = "inventory_4",
            .CurrentMoney = 1000,
            .Items = {
                app_domain::InventoryItem{.ItemId = "healing_potion", .Count = 1, .ModifiedAt = 100 },
                app_domain::InventoryItem{.ItemId = "shield", .Count = 1, .ModifiedAt = 50 },
                app_domain::InventoryItem{.ItemId = "mana_potion", .Count = 1, .ModifiedAt = 100 },
                app_domain::InventoryItem{.ItemId = "sword", .Count = 1, .ModifiedAt = 50 },
                app_domain::InventoryItem{.ItemId = "healing_potion", .Count = 1, .ModifiedAt = 100 }
            }
        }},
        { "inventory_5", app_domain::Inventory{
            .Id = "inventory_5",
            .CurrentMoney = 1000,
            .Items = {
                app_domain::InventoryItem{.ItemId = "sword", .Count = 1, .ModifiedAt = 100 },
                app_domain::InventoryItem{.ItemId = "scroll", .Count = 1, .ModifiedAt = 50 },
                app_domain::InventoryItem{.ItemId = "tool", .Count = 1, .ModifiedAt = 100 },
            }
        }},
        { "inventory_6", app_domain::Inventory{
            .Id = "inventory_6",
            .CurrentMoney = 1000,
            .Items = {
                app_domain::InventoryItem{.ItemId = "key", .Count = 1, .ModifiedAt = 50 },
                app_domain::InventoryItem{.ItemId = "nonexistent_item", .Count = 1, .ModifiedAt = 100 },
                app_domain::InventoryItem{.ItemId = "book", .Count = 2, .ModifiedAt = 100 },
                app_domain::InventoryItem{.ItemId = "diamond", .Count = 2, .ModifiedAt = 100 },
            }
        }},
    };

    app_domain::ItemService ItemService{ Items };

    app_domain::CharacterService CharacterService{ Characters };

    app_domain::InventoryService InventoryService{ Inventories, ItemService };

    app_domain::TradeService TradeService{ ItemService, CharacterService, InventoryService };
};