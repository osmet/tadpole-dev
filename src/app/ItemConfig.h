#pragma once

#include "ItemData.h"
#include "../core/IConfig.h"

namespace app
{
    class ItemConfig : public core::IConfig
    {
    public:
        void FromJson(const rapidjson::Value& json);

        const ItemData& GetItemById(const std::string& id) const;
        const std::unordered_map<std::string, ItemData>& GetItems() const;

    private:
        std::unordered_map<std::string, ItemData> m_items;
    };
}