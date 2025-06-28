#pragma once

#include "../core/IJsonData.h"

namespace app
{
    class AppData : public core::IJsonData
    {
    public:
        void FromJson(const rapidjson::Value& json) override;
        void ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

        const std::string& GetLastSavedAt() const;

    private:
        std::string m_lastSavedAt;
    };
}