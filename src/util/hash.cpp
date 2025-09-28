#include "hash.h"
#include "macros.h"
#include "types.h"

#include <chrono>
#include <thread>

EMB_NAMESPACE_START

embGuid Hash::GenerateGUID() noexcept
{
    // grabs epoch time and thread id to generate GUID.

    using clock = std::chrono::high_resolution_clock;
    using nanosec = std::chrono::nanoseconds;
    std::hash<std::thread::id> hasher;

    embGuid ret = std::chrono::duration_cast<nanosec>(clock::now().time_since_epoch()).count();
    embGuid tval = hasher(std::this_thread::get_id());

    //constexpr embU64 halfSize = (sizeof(embGenericGuid) * CHAR_BIT) / 2;

    // // replace lower half bits with thread ID in the rare case of 2 threads calling at same time.
    // ret <<= halfSize;
    // tval = (tval << halfSize) >> halfSize;
    // return ret | tval;

    // xor both bois
    return ret ^ tval;
}

embGuid64 Hash::GenerateGUID64() noexcept
{
    // grabs epoch time and thread id to generate GUID.

    using clock = std::chrono::high_resolution_clock;
    using nanosec = std::chrono::nanoseconds;
    std::hash<std::thread::id> hasher;

    embGuid64 ret = std::chrono::duration_cast<nanosec>(clock::now().time_since_epoch()).count();
    embGuid64 tval = hasher(std::this_thread::get_id());

    //constexpr embU64 halfSize = (sizeof(embGenericGuid) * CHAR_BIT) / 2;

    // // replace lower half bits with thread ID in the rare case of 2 threads calling at same time.
    // ret <<= halfSize;
    // tval = (tval << halfSize) >> halfSize;
    // return ret | tval;

    // xor both bois
    return ret ^ tval;
}

EMB_NAMESPACE_END
