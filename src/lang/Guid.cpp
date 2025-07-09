#include "Precompiled.h"
#include "Guid.h"

#include <format>
#include <stdexcept>
#include <objbase.h>

namespace lang
{
    Guid Guid::New()
    {
        GUID guid;

        if (CoCreateGuid(&guid) != S_OK)
            throw std::runtime_error("Failed to generate GUID");

        return Guid(guid);
    }

    Guid::Guid(const GUID& guid) : m_guid(guid)
    {
    }

    bool Guid::operator==(const Guid& other) const
    {
        return ::IsEqualGUID(m_guid, other.m_guid) != FALSE;
    }

    bool Guid::operator!=(const Guid& other) const
    {
        return !(*this == other);
    }

    std::string Guid::ToString() const
    {
        return std::format("{:08X}-{:04X}-{:04X}-{:02X}{:02X}-{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
            m_guid.Data1, m_guid.Data2, m_guid.Data3,
            m_guid.Data4[0], m_guid.Data4[1],
            m_guid.Data4[2], m_guid.Data4[3], m_guid.Data4[4],
            m_guid.Data4[5], m_guid.Data4[6], m_guid.Data4[7]
        );
    }
}