#pragma once

#include "../core/IJsonData.h"

namespace app
{
    struct AppData : core::IJsonData
    {
        void FromJson(const rapidjson::Value& json) override;
        void ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    };
}