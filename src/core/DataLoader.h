#pragma once

#include "IData.h"

namespace core
{
    enum class DataLoaderError : int8_t
    {
        FilePathNotFound = 0,
        FileOpenFailed = 1,
        InvalidDataType = 2,
        DataParseFailed = 3
    };

    class DataLoader
    {
    public:
        tl::expected<void, DataLoaderError> Load(IData& data, const std::string& filePath) const;
        tl::expected<void, DataLoaderError> Load(IData& data, const std::string& filePath, const std::string& fallbackFilePath) const;
        tl::expected<void, DataLoaderError> Save(const IData& data, const std::string& filePath) const;

    protected:
        virtual tl::expected<void, DataLoaderError> LoadFromStream(std::istream& input, IData& data) const = 0;
        virtual tl::expected<void, DataLoaderError> SaveToStream(std::ostream& output, const IData& data) const = 0;
    };
}
