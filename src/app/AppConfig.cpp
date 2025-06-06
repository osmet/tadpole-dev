#include "Precompiled.h"
#include "AppConfig.h"

namespace app
{
    void AppConfig::FromJson(const rapidjson::Value& json)
    {
        assert(json.HasMember("app_name"));
        assert(json["app_name"].IsString());
        m_appName = json["app_name"].GetString();

        assert(json.HasMember("window_width"));
        assert(json["window_width"].IsUint());
        m_windowWidth = json["window_width"].GetUint();

        assert(json.HasMember("window_height"));
        assert(json["window_height"].IsUint());
        m_windowHeight = json["window_height"].GetUint();

        assert(json.HasMember("window_style"));
        assert(json["window_style"].IsString());
        std::string windowStyleString = json["window_style"].GetString();

        if (windowStyleString == "fullscreen")
        {
            m_windowStyle = sf::Style::Fullscreen;
        }
        else
        {
            m_windowStyle = sf::Style::Default;
        }
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