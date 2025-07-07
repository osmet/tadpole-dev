#include "Precompiled.h"
#include "JsonDataLoader.h"
#include "IJsonData.h"

namespace core
{
    lang::Expected<void, DataLoaderError> JsonDataLoader::LoadFromStream(std::istream& input, IData& data) const
    {
        auto* jsonData = dynamic_cast<IJsonData*>(&data);
        if (!jsonData)
            return tl::unexpected(DataLoaderError::InvalidDataType);

        rapidjson::IStreamWrapper streamWrapper(input);
        rapidjson::Document document;
        document.ParseStream(streamWrapper);

        if (document.HasParseError() || !document.IsObject())
            return tl::unexpected(DataLoaderError::DataParseFailed);

        jsonData->FromJson(document);

        return {};
    }

    lang::Expected<void, DataLoaderError> JsonDataLoader::SaveToStream(std::ostream& output, const IData& data) const
    {
        const auto* jsonData = dynamic_cast<const IJsonData*>(&data);
        if (!jsonData)
            return tl::unexpected(DataLoaderError::InvalidDataType);

        rapidjson::Document document;
        document.SetObject();

        jsonData->ToJson(document, document.GetAllocator());

        rapidjson::OStreamWrapper streamWrapper(output);
        rapidjson::Writer<rapidjson::OStreamWrapper> writer(streamWrapper);

        document.Accept(writer);

        return {};
    }
}
