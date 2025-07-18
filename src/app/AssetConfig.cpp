#include "Precompiled.h"
#include "AssetConfig.h"

namespace app
{
    void AssetConfig::FromJson(const rapidjson::Value& json)
    {
        if (json.HasMember("Fonts"))
        {
            const auto& fonts = json["Fonts"];

            for (auto it = fonts.MemberBegin(); it != fonts.MemberEnd(); ++it)
            {
                assert(it->name.IsString());
                assert(it->value.IsString());

                std::string fontId = it->name.GetString();
                std::string fontPath = it->value.GetString();

                m_fontPaths[fontId] = fontPath;
            }
        }

        if (json.HasMember("Textures"))
        {
            const auto& textures = json["Textures"];

            for (auto it = textures.MemberBegin(); it != textures.MemberEnd(); ++it)
            {
                assert(it->name.IsString());
                assert(it->value.IsString());

                std::string textureId = it->name.GetString();
                std::string texturePath = it->value.GetString();

                m_texturePaths[textureId] = texturePath;
            }
        }
    }

    const std::unordered_map<std::string, std::string>& AssetConfig::GetFontPaths() const
    {
        return m_fontPaths;
    }

    const std::unordered_map<std::string, std::string>& AssetConfig::GetTexturePaths() const
    {
        return m_texturePaths;
    }
}