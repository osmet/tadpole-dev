#pragma once

#include "ItemData.h"
#include "../core/IConfig.h"

namespace app
{
    class ItemConfig : public core::IConfig
    {
    public:
        void FromJson(const rapidjson::Value& json);

        const ItemDataMap& GetItems() const;

    private:
        ItemDataMap m_items;
    };
}