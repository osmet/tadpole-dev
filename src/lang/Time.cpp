#include "Precompiled.h"
#include "Time.h"
#include <chrono>

namespace lang
{
    std::uint64_t Time::GetUnixTimeMilliseconds()
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }
}