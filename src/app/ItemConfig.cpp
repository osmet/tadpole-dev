#include "Precompiled.h"
#include "ItemConfig.h"

#include "Precompiled.h"
#include "ItemConfig.h"

namespace app
{
    const sf::Color& ItemConfig::GetRarityColor(app_domain::ItemRarity rarity)
    {
        switch (rarity)
        {
        case app_domain::ItemRarity::Uncommon:
            return sf::Color(0u, 204u, 20u, 255u);
        case app_domain::ItemRarity::Rare:
            return sf::Color(64u, 191u, 255u, 255u);
        case app_domain::ItemRarity::VeryRare:
            return sf::Color(255u, 64u, 160u, 255u);
        case app_domain::ItemRarity::Legendary:
            return sf::Color(255u, 175u, 64u, 255u);
        default:
            return sf::Color::White;
        }
    }

    void ItemConfig::FromJson(const rapidjson::Value& json)
    {
        assert(json.HasMember("Items"));
        const auto& arrayObject = json["Items"];
        assert(arrayObject.IsArray());

        m_items.clear();

        for (auto index = 0u; index < arrayObject.Size(); ++index)
        {
            const auto& value = arrayObject[index];

            assert(value.HasMember("Id") && value["Id"].IsString());
            assert(value.HasMember("Name") && value["Name"].IsString());
            assert(value.HasMember("IconTextureId") && value["IconTextureId"].IsString());
            assert(value.HasMember("Description") && value["Description"].IsString());
            assert(value.HasMember("Type") && value["Type"].IsString());
            assert(value.HasMember("IsStoryItem") && value["IsStoryItem"].IsBool());
            assert(value.HasMember("Rarity") && value["Rarity"].IsString());
            assert(value.HasMember("Weight") && value["Weight"].IsNumber());
            assert(value.HasMember("Value") && value["Value"].IsInt());

            app_domain::Item item;
            item.Id = value["Id"].GetString();
            item.Name = value["Name"].GetString();
            item.IconTextureId = value["IconTextureId"].GetString();
            item.Description = value["Description"].GetString();
            item.Type = app_domain::ItemTypeHelper::FromString(value["Type"].GetString());
            item.IsStoryItem = value["IsStoryItem"].GetBool();
            item.Rarity = app_domain::ItemRarityHelper::FromString(value["Rarity"].GetString());
            item.Weight = value["Weight"].GetFloat();
            item.Value = value["Value"].GetInt();

            m_items.emplace(item.Id, std::move(item));
        }
    }

    const app_domain::ItemMap& ItemConfig::GetItems() const
    {
        return m_items;
    }
}