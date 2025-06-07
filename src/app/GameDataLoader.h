#pragma once

#include "GameData.h"
#include "../core/JsonDataLoader.h"

namespace app
{
    class GameDataLoader : public core::JsonDataLoader
    {
    public:
        GameDataLoader(GameData& data, const std::string& rootPath, const std::string& filePath);

    protected:
        void FromJson(const rapidjson::Value& json) override;
        void ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) override;

    private:
        GameData& m_data;
    };
}