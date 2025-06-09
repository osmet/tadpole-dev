#pragma once

#include "ItemData.h"
#include "../core/IJsonData.h"

namespace app
{
    class ItemConfig : public core::IJsonData
    {
    public:
        void FromJson(const rapidjson::Value& json);

        const ItemDataMap& GetItems() const;

    private:
        ItemDataMap m_items;
    };
}