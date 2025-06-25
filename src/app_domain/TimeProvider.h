#pragma once

namespace app_domain
{
    class TimeProvider
    {
    public:
        static std::uint64_t GetUnixTimeMilliseconds();
    };
}

