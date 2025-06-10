#pragma once

#include "../core/IJsonData.h"
#include "../app_domain/Character.h"
#include "../app_domain/Inventory.h"

namespace app
{
    class GameData : public core::IJsonData
    {
    public:
        void FromJson(const rapidjson::Value& json) override;
        void ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

        app_domain::CharacterMap& GetCharacters();
        app_domain::InventoryMap& GetInventories();

    private:
        app_domain::CharacterMap m_characters;
        app_domain::InventoryMap m_inventories;
    };
}