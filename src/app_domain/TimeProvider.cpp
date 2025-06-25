#include "Precompiled.h"
#include "TimeProvider.h"
#include <chrono>

namespace app_domain
{
    std::uint64_t TimeProvider::GetUnixTimeMilliseconds()
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }
}