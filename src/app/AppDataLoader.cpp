#include "Precompiled.h"
#include "AppDataLoader.h"

#include <chrono>
#include <iomanip>

namespace app
{
    AppDataLoader::AppDataLoader(AppData& data, const std::string& rootPath, const std::string& filePath)
        : JsonDataLoader(rootPath, filePath), m_data(data)
    {
    }

    void AppDataLoader::FromJson(const rapidjson::Value& json)
    {
    }

    void AppDataLoader::ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator)
    {
    }
}
