#include "Precompiled.h"
#include "AppDataLoader.h"

#include <chrono>
#include <iomanip>

namespace app
{
    std::string GetCurrentTimeISO()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);

        std::tm utcTime{};
        gmtime_s(&utcTime, &time);

        std::ostringstream oss;
        oss << std::put_time(&utcTime, "%d-%m-%Y %H:%M:%S");

        return oss.str();
    }

    AppDataLoader::AppDataLoader(AppData& data, const std::string& rootPath, const std::string& filePath)
        : JsonDataLoader(rootPath, filePath), m_data(data)
    {
    }

    void AppDataLoader::FromJson(const rapidjson::Value& json)
    {
        assert(json.HasMember("last_save_time") && json["last_save_time"].IsString());

        m_data.LastSaveTime = json["last_save_time"].GetString();
    }

    void AppDataLoader::ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator)
    {
        std::string isoTime = GetCurrentTimeISO();

        json.AddMember("last_save_time", rapidjson::Value(isoTime.c_str(), allocator), allocator);
    }
}
