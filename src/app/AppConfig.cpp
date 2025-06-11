#include "Precompiled.h"
#include "AppConfig.h"

namespace app
{
    void AppConfig::FromJson(const rapidjson::Value& json)
    {
        assert(json.HasMember("AppName"));
        assert(json["AppName"].IsString());
        m_appName = json["AppName"].GetString();

        assert(json.HasMember("WindowWidth"));
        assert(json["WindowWidth"].IsUint());
        m_windowWidth = json["WindowWidth"].GetUint();

        assert(json.HasMember("WindowHeight"));
        assert(json["WindowHeight"].IsUint());
        m_windowHeight = json["WindowHeight"].GetUint();

        assert(json.HasMember("WindowStyle"));
        assert(json["WindowStyle"].IsString());
        std::string windowStyleString = json["WindowStyle"].GetString();

        if (windowStyleString == "Fullscreen")
            m_windowStyle = sf::Style::Fullscreen;
        else
            m_windowStyle = sf::Style::Default;
    }

    const std::string& AppConfig::GetAppName() const
    {
        return m_appName;
    }

    std::uint32_t AppConfig::GetAppWindowWidth() const
    {
        return m_windowWidth;
    }

    std::uint32_t AppConfig::GetAppWindowHeight() const
    {
        return m_windowHeight;
    }

    std::uint32_t AppConfig::GetAppWindowStyle() const
    {
        return m_windowStyle;
    }
}