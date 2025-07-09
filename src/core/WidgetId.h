#pragma once

#include "../lang/Guid.h"

namespace core
{
    class WidgetId
    {
    public:
        static WidgetId New();

        WidgetId() = default;

        bool operator==(const WidgetId& other) const = default;

        std::string ToString() const;

    private:
        WidgetId(const lang::Guid& guid);

        lang::Guid m_guid{};
    };
}
