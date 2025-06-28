#include "Precompiled.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include "AppData.h"

namespace app
{
    std::string GetCurrentDateTime()
    {
        std::time_t now = std::time(nullptr);
        std::tm localTime{};

        localtime_s(&localTime, &now);

        std::ostringstream oss;
        oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

        return oss.str();
    }

    void AppData::FromJson(const rapidjson::Value& json)
    {
        if (json.HasMember("LastSavedAt") && json["LastSavedAt"].IsString())
        {
            m_lastSavedAt = json["LastSavedAt"].GetString();
        }
    }

    void AppData::ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const
    {
        json.AddMember("LastSavedAt", rapidjson::Value(GetCurrentDateTime().c_str(), allocator), allocator);
    }

    const std::string& AppData::GetLastSavedAt() const
    {
        return m_lastSavedAt;
    }
}
