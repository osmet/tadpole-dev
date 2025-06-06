#pragma once

#include "AppData.h"
#include "../core/JsonDataLoader.h"

namespace app
{
    class AppDataLoader : public core::JsonDataLoader
    {
    public:
        AppDataLoader(AppData& data, const std::string& rootPath, const std::string& filePath);

    protected:
        void FromJson(const rapidjson::Value& json) override;
        void ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) override;

    private:
        AppData& m_data;
    };
}