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
        lang::Expected<void, DataLoaderError> Load(IData& data, const std::string& filePath) const;
        lang::Expected<void, DataLoaderError> Load(IData& data, const std::string& filePath, const std::string& fallbackFilePath) const;
        lang::Expected<void, DataLoaderError> Save(const IData& data, const std::string& filePath) const;

    protected:
        virtual lang::Expected<void, DataLoaderError> LoadFromStream(std::istream& input, IData& data) const = 0;
        virtual lang::Expected<void, DataLoaderError> SaveToStream(std::ostream& output, const IData& data) const = 0;
    };
}
