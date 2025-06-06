#pragma once

#include "IConfig.h"

namespace core
{
    class ConfigManager
    {
    public:
        ConfigManager(const std::string& rootPath)
            : m_rootPath(rootPath)
        {
            assert(!m_rootPath.empty() && "Root path must not be empty.");
        }

        template <typename TData>
        void LoadConfig(const std::string& filePath)
        {
            static_assert(std::is_base_of_v<IConfig, TData>, "T must implement IConfig.");

            assert(!filePath.empty() && "File path must not be empty.");
            assert(m_configs.find(std::type_index(typeid(TData))) == m_configs.end() && "Config is already loaded.");

            std::string configPath = m_rootPath + filePath;
            std::ifstream file(configPath);
            assert(file.is_open() && "Failed to open the file.");

            rapidjson::IStreamWrapper streamWrapper(file);
            rapidjson::Document document;

            document.ParseStream(streamWrapper);
            assert(!document.HasParseError() && "Failed to parse the JSON file.");
            assert(document.IsObject() && "Parsed JSON is not an object.");

            auto config = std::make_unique<TData>();

            config->FromJson(document);

            m_configs[std::type_index(typeid(TData))] = std::move(config);
        }

        template <typename TData>
        const TData& Get() const
        {
            static_assert(std::is_base_of_v<IConfig, TData>, "TData must implement IConfig.");

            auto it = m_configs.find(std::type_index(typeid(TData)));
            assert(it != m_configs.end() && "Config is not loaded.");

            return *static_cast<TData*>(it->second.get());
        }

    private:
        std::string m_rootPath;
        std::unordered_map<std::type_index, std::unique_ptr<IConfig>> m_configs;
    };
}