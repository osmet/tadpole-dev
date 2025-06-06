#include "Precompiled.h"
#include "JsonDataLoader.h"

namespace core
{
    JsonDataLoader::JsonDataLoader(const std::string& rootPath, const std::string& filePath)
    {
        assert(!rootPath.empty() && "Root path must not be empty.");
        assert(!filePath.empty() && "File path must not be empty.");

        m_dataPath = filePath;
        m_defaultDataPath = rootPath + filePath;
    }

    void JsonDataLoader::Load()
    {
        std::string path;

        if (std::filesystem::exists(m_dataPath))
            path = m_dataPath;
        else if (std::filesystem::exists(m_defaultDataPath))
            path = m_defaultDataPath;
        else
            assert(false && "Both data and default files are missing.");

        std::ifstream file(path);
        assert(file.is_open() && "Failed to open JSON file for reading.");

        rapidjson::IStreamWrapper streamWrapper(file);
        rapidjson::Document document;
        document.ParseStream(streamWrapper);

        assert(!document.HasParseError() && "Failed to parse the JSON file.");
        assert(document.IsObject() && "Parsed JSON is not an object.");

        FromJson(document);
    }

    void JsonDataLoader::Save()
    {
        rapidjson::Document document;
        document.SetObject();

        ToJson(document, document.GetAllocator());

        std::ofstream file(m_dataPath);
        assert(file.is_open() && "Failed to open JSON file for writing.");

        rapidjson::OStreamWrapper streamWrapper(file);
        rapidjson::Writer<rapidjson::OStreamWrapper> writer(streamWrapper);

        document.Accept(writer);
    }
}
