#pragma once

#include "../app_domain/Item.h"
#include "../core/IJsonData.h"

namespace app
{
    class ItemConfig : public core::IJsonData
    {
    public:
        static const sf::Color GetRarityColor(app_domain::ItemRarity rarity);

        void FromJson(const rapidjson::Value& json);

        const app_domain::ItemMap& GetItems() const;


    private:
        app_domain::ItemMap m_items;
    };
}