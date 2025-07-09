#include "Precompiled.h"
#include "WidgetId.h"

namespace core
{
    WidgetId WidgetId::New()
    {
        return WidgetId(lang::Guid::New());
    }

    std::string WidgetId::ToString() const
    {
        return m_guid.ToString();
    }

    WidgetId::WidgetId(const lang::Guid& guid)
        : m_guid(guid)
    {
    }
}