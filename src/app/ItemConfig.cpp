#include "Precompiled.h"
#include "ItemConfig.h"

#include "Precompiled.h"
#include "ItemConfig.h"

namespace app
{
    void ItemConfig::FromJson(const rapidjson::Value& json)
    {
        assert(json.HasMember("items"));
        const auto& arrayObject = json["items"];
        assert(arrayObject.IsArray());

        m_items.clear();
        
        for (auto index = 0u; index < arrayObject.Size(); ++index)
        {
            const auto& value = arrayObject[index];

            assert(value.HasMember("id") && value["id"].IsString());
            assert(value.HasMember("name") && value["name"].IsString());
            assert(value.HasMember("icon_texture_id") && value["icon_texture_id"].IsString());
            assert(value.HasMember("description") && value["description"].IsString());
            assert(value.HasMember("type") && value["type"].IsString());
            assert(value.HasMember("is_story_item") && value["is_story_item"].IsBool());
            assert(value.HasMember("rarity") && value["rarity"].IsString());
            assert(value.HasMember("weight") && value["weight"].IsNumber());
            assert(value.HasMember("price") && value["price"].IsInt());

            app_domain::Item item;
            item.Id = value["id"].GetString();
            item.Name = value["name"].GetString();
            item.IconTextureId = value["icon_texture_id"].GetString();
            item.Description = value["description"].GetString();
            item.Type = app_domain::ItemTypeHelper::FromString(value["type"].GetString());
            item.IsStoryItem = value["is_story_item"].GetBool();
            item.Rarity = app_domain::ItemRarityHelper::FromString(value["rarity"].GetString());
            item.Weight = value["weight"].GetFloat();
            item.Price = value["price"].GetInt();

            m_items.emplace(item.Id, std::move(item));
        }
    }

    const app_domain::ItemMap& ItemConfig::GetItems() const
    {
        return m_items;
    }
}