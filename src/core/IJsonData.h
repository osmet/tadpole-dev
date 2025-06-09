#pragma once

#include "IData.h"

namespace core
{
    class IJsonData : public IData
    {
    public:
        ~IJsonData() override = 0;

        virtual void FromJson(const rapidjson::Value& json) = 0;
        virtual void ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const;
    };
}
