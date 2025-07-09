#pragma once

#include <rpc.h>
#include <string>

namespace lang
{
    class Guid
    {
    public:
        static Guid New();

        Guid() = default;

        bool operator==(const Guid& other) const;
        bool operator!=(const Guid& other) const;

        std::string ToString() const;

    private:
        explicit Guid(const GUID& guid);

        GUID m_guid{};
    };
}