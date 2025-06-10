#include "Precompiled.h"
#include "GameData.h"

namespace app
{
    void GameData::FromJson(const rapidjson::Value& json)
    {
        assert(json.HasMember("characters") && json["characters"].IsObject());

        for (auto it = json["characters"].MemberBegin(); it != json["characters"].MemberEnd(); ++it)
        {
            const std::string id = it->name.GetString();
            const auto& value = it->value;

            assert(value.HasMember("name") && value["name"].IsString());
            assert(value.HasMember("portrait_texture_id") && value["portrait_texture_id"].IsString());
            assert(value.HasMember("inventory_id") && value["inventory_id"].IsString());
            assert(value.HasMember("max_weight") && value["max_weight"].IsNumber());

            app_domain::Character character;
            character.Id = id;
            character.Name = value["name"].GetString();
            character.PortraitTextureId = value["portrait_texture_id"].GetString();
            character.MaxWeight = value["max_weight"].GetFloat();
            character.InventoryId = value["inventory_id"].GetString();

            m_characters.emplace(id, std::move(character));
        }

        assert(json.HasMember("inventories"));
        assert(json["inventories"].IsObject());

        for (auto it = json["inventories"].MemberBegin(); it != json["inventories"].MemberEnd(); ++it)
        {
            const std::string id = it->name.GetString();
            const auto& value = it->value;

            assert(value.HasMember("items") && value["items"].IsArray());
            assert(value.HasMember("current_money") && value["current_money"].IsInt());

            app_domain::Inventory inventory;
            inventory.Id = id;
            inventory.CurrentMoney = value["current_money"].GetInt();

            for (const auto& item : value["items"].GetArray())
            {
                assert(item.HasMember("item_id") && item["item_id"].IsString());
                assert(item.HasMember("count") && item["count"].IsInt());

                app_domain::InventoryItem inventoryItem;
                inventoryItem.ItemId = item["item_id"].GetString();
                inventoryItem.Count = item["count"].GetInt();

                inventory.Items.push_back(std::move(inventoryItem));
            }

            m_inventories.emplace(id, std::move(inventory));
        }
    }

    void GameData::ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const
    {
        rapidjson::Value charactersObj(rapidjson::kObjectType);
        for (const auto& [id, character] : m_characters)
        {
            rapidjson::Value obj(rapidjson::kObjectType);
            obj.AddMember("name", rapidjson::Value(character.Name.c_str(), allocator), allocator);
            obj.AddMember("portrait_texture_id", rapidjson::Value(character.PortraitTextureId.c_str(), allocator), allocator);
            obj.AddMember("max_weight", character.MaxWeight, allocator);
            obj.AddMember("inventory_id", rapidjson::Value(character.InventoryId.c_str(), allocator), allocator);

            charactersObj.AddMember(rapidjson::Value(id.c_str(), allocator), obj, allocator);
        }
        json.AddMember("characters", charactersObj, allocator);

        rapidjson::Value inventoriesObj(rapidjson::kObjectType);
        for (const auto& [id, inventory] : m_inventories)
        {
            rapidjson::Value items(rapidjson::kArrayType);
            for (const auto& item : inventory.Items)
            {
                rapidjson::Value itemObj(rapidjson::kObjectType);
                itemObj.AddMember("item_id", rapidjson::Value(item.ItemId.c_str(), allocator), allocator);
                itemObj.AddMember("count", item.Count, allocator);
                items.PushBack(itemObj, allocator);
            }

            rapidjson::Value inventoryObj(rapidjson::kObjectType);
            inventoryObj.AddMember("current_money", inventory.CurrentMoney, allocator);
            inventoryObj.AddMember("items", items, allocator);

            inventoriesObj.AddMember(rapidjson::Value(id.c_str(), allocator), inventoryObj, allocator);
        }
        json.AddMember("inventories", inventoriesObj, allocator);
    }

    app_domain::CharacterMap& GameData::GetCharacters()
    {
        return m_characters;
    }

    app_domain::InventoryMap& GameData::GetInventories()
    {
        return m_inventories;
    }
}