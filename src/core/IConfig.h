#pragma once

namespace core
{
    class IConfig
    {
    public:
        virtual ~IConfig() = 0;

        virtual void FromJson(const rapidjson::Value& json) = 0;
    };
}