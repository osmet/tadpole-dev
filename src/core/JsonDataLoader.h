#pragma once

#include "DataLoader.h"

namespace core
{
    class JsonDataLoader final : public DataLoader
    {
    protected:
        tl::expected<void, DataLoaderError> LoadFromStream(std::istream& input, IData& data) const override;
        tl::expected<void, DataLoaderError> SaveToStream(std::ostream& output, const IData& data) const override;
    };
}