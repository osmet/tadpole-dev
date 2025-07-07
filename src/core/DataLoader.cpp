#include "Precompiled.h"
#include "DataLoader.h"

namespace core
{
    lang::Expected<void, DataLoaderError> DataLoader::Load(IData& data, const std::string& filePath) const
    {
        if (filePath.empty() || !std::filesystem::exists(filePath))
            return tl::unexpected(DataLoaderError::FilePathNotFound);

        std::ifstream file(filePath);
        if (!file.is_open())
            return tl::unexpected(DataLoaderError::FileOpenFailed);

        return LoadFromStream(file, data);
    }

    lang::Expected<void, DataLoaderError> DataLoader::Load(
        IData& data,
        const std::string& filePath,
        const std::string& fallbackFilePath) const
    {
        if (!filePath.empty() && std::filesystem::exists(filePath))
            return Load(data, filePath);

        if (!fallbackFilePath.empty() && std::filesystem::exists(fallbackFilePath))
            return Load(data, fallbackFilePath);

        return tl::unexpected(DataLoaderError::FilePathNotFound);
    }

    lang::Expected<void, DataLoaderError> DataLoader::Save(const IData& data, const std::string& filePath) const
    {
        if (filePath.empty())
            return tl::unexpected(DataLoaderError::FilePathNotFound);

        std::ofstream file(filePath);
        if (!file.is_open())
            return tl::unexpected(DataLoaderError::FileOpenFailed);

        return SaveToStream(file, data);
    }
}