#pragma once

#include "../core/IJsonData.h"
#include "CharacterData.h"
#include "InventoryData.h"

namespace app
{
    class GameData : public core::IJsonData
    {
    public:
        void FromJson(const rapidjson::Value& json) override;
        void ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

        CharacterDataMap& GetCharacters();
        InventoryDataMap& GetInventories();

    private:
        CharacterDataMap m_characters;
        InventoryDataMap m_inventories;
    };
}