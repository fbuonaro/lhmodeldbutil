#ifndef __CUSTOM_TYPES_H__
#define __CUSTOM_TYPES_H__

#include <chrono>
#include <cstdint>

namespace LHModelDbUtilTestNS
{
    typedef uint64_t Uuid;

    typedef std::chrono::system_clock::time_point PointInTime;
}

#endif
