#pragma once

namespace core
{
    class JsonDataLoader
    {
    public:
        JsonDataLoader(const std::string& rootPath, const std::string& filePath);
        void Load();
        void Save();

    protected:
        virtual void FromJson(const rapidjson::Value& json) = 0;
        virtual void ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) = 0;

        std::string m_dataPath;
        std::string m_defaultDataPath;
    };
}