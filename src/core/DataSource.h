#pragma once

#include "IData.h"
#include "DataLoader.h"

namespace core
{
    template<typename TData, typename TDataLoader>
    class DataSource
    {
        static_assert(std::is_base_of<IData, TData>::value, "TData must derive from core::IData");
        static_assert(std::is_base_of<DataLoader, TDataLoader>::value, "TDataLoader must derive from core::DataLoader");

    public:
        DataSource(std::string filePath, std::string fallbackFilePath)
            : m_filePath(std::move(filePath))
            , m_fallbackFilePath(std::move(fallbackFilePath))
        {
        }

        TData& GetData() { return m_data; }

        const std::string& GetFilePath() const { return m_filePath; }
        const std::string& GetFallbackFilePath() const { return m_fallbackFilePath; }

        lang::Expected<void, DataLoaderError> Load()
        {
            return m_dataLoader.Load(m_data, m_filePath, m_fallbackFilePath);
        }

        lang::Expected<void, DataLoaderError> Save() const
        {
            return m_dataLoader.Save(m_data, m_filePath);
        }

    private:
        TData m_data;
        TDataLoader m_dataLoader;
        std::string m_filePath;
        std::string m_fallbackFilePath;
    };
}
