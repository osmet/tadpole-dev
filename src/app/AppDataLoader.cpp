#include "Precompiled.h"
#include "AppDataLoader.h"

namespace app
{
    AppDataLoader::AppDataLoader(AppData& data, const std::string& rootPath, const std::string& filePath)
        : JsonDataLoader(rootPath, filePath), m_data(data)
    {
    }

    void AppDataLoader::FromJson(const rapidjson::Value& json)
    {
        if (json.HasMember("name") && json["name"].IsString())
        {
            m_data.Name = json["name"].GetString();
        }
    }

    void AppDataLoader::ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator)
    {
        json.AddMember("name", rapidjson::Value(m_data.Name.c_str(), allocator), allocator);
    }
}
