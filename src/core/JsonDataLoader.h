#pragma once

#include "DataLoader.h"

namespace core
{
    class JsonDataLoader final : public DataLoader
    {
    protected:
        lang::Expected<void, DataLoaderError> LoadFromStream(std::istream& input, IData& data) const override;
        lang::Expected<void, DataLoaderError> SaveToStream(std::ostream& output, const IData& data) const override;
    };
}