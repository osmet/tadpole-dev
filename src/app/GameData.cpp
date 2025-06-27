#include "Precompiled.h"
#include "GameData.h"

namespace app
{
    void GameData::FromJson(const rapidjson::Value& json)
    {
        assert(json.HasMember("Characters") && json["Characters"].IsObject());

        for (auto it = json["Characters"].MemberBegin(); it != json["Characters"].MemberEnd(); ++it)
        {
            const std::string id = it->name.GetString();
            const auto& value = it->value;

            assert(value.HasMember("Name") && value["Name"].IsString());
            assert(value.HasMember("PortraitTextureId") && value["PortraitTextureId"].IsString());
            assert(value.HasMember("InventoryId") && value["InventoryId"].IsString());
            assert(value.HasMember("MaxWeight") && value["MaxWeight"].IsNumber());

            app_domain::Character character;
            character.Id = id;
            character.Name = value["Name"].GetString();
            character.PortraitTextureId = value["PortraitTextureId"].GetString();
            character.MaxWeight = value["MaxWeight"].GetFloat();
            character.InventoryId = value["InventoryId"].GetString();

            m_characters.emplace(id, std::move(character));
        }

        assert(json.HasMember("Inventories"));
        assert(json["Inventories"].IsObject());

        for (auto it = json["Inventories"].MemberBegin(); it != json["Inventories"].MemberEnd(); ++it)
        {
            const std::string id = it->name.GetString();
            const auto& value = it->value;

            assert(value.HasMember("Items") && value["Items"].IsArray());
            assert(value.HasMember("CurrentMoney") && value["CurrentMoney"].IsInt());

            app_domain::Inventory inventory;
            inventory.Id = id;
            inventory.CurrentMoney = value["CurrentMoney"].GetInt();

            rapidjson::SizeType itemCount = value["Items"].Size();

            for (rapidjson::SizeType itemIndex = 0; itemIndex < itemCount; ++itemIndex)
            {
                const auto& item = value["Items"][itemIndex];

                assert(item.HasMember("ItemId") && item["ItemId"].IsString());
                assert(item.HasMember("Count") && item["Count"].IsInt());

                app_domain::InventoryItem inventoryItem;
                inventoryItem.ItemId = item["ItemId"].GetString();
                inventoryItem.Count = item["Count"].GetInt();

                if (item.HasMember("ModifiedAt") && item["ModifiedAt"].IsUint64())
                    inventoryItem.ModifiedAt = item["ModifiedAt"].GetUint64();
                else
                    inventoryItem.ModifiedAt = static_cast<uint64_t>(itemCount - itemIndex - 1u); // Fallback: Get ModifiedAt by reverse item index

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
            obj.AddMember("Name", rapidjson::Value(character.Name.c_str(), allocator), allocator);
            obj.AddMember("PortraitTextureId", rapidjson::Value(character.PortraitTextureId.c_str(), allocator), allocator);
            obj.AddMember("MaxWeight", character.MaxWeight, allocator);
            obj.AddMember("InventoryId", rapidjson::Value(character.InventoryId.c_str(), allocator), allocator);

            charactersObj.AddMember(rapidjson::Value(id.c_str(), allocator), obj, allocator);
        }
        json.AddMember("Characters", charactersObj, allocator);

        rapidjson::Value inventoriesObj(rapidjson::kObjectType);
        for (const auto& [id, inventory] : m_inventories)
        {
            rapidjson::Value items(rapidjson::kArrayType);
            for (const auto& item : inventory.Items)
            {
                rapidjson::Value itemObj(rapidjson::kObjectType);
                itemObj.AddMember("ItemId", rapidjson::Value(item.ItemId.c_str(), allocator), allocator);
                itemObj.AddMember("Count", item.Count, allocator);
                itemObj.AddMember("ModifiedAt", item.ModifiedAt, allocator);

                items.PushBack(itemObj, allocator);
            }

            rapidjson::Value inventoryObj(rapidjson::kObjectType);
            inventoryObj.AddMember("CurrentMoney", inventory.CurrentMoney, allocator);
            inventoryObj.AddMember("Items", items, allocator);

            inventoriesObj.AddMember(rapidjson::Value(id.c_str(), allocator), inventoryObj, allocator);
        }
        json.AddMember("Inventories", inventoriesObj, allocator);
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