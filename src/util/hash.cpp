#include "hash.h"
#include "macros.h"
#include "types.h"

#include <chrono>
#include <thread>

EMB_NAMESPACE_START

embGenericGUID Hash::GenerateGUID() noexcept
{
    using clock = std::chrono::high_resolution_clock;
    using nanosec = std::chrono::nanoseconds;
    std::hash<std::thread::id> hasher;

    embGenericGUID ret = std::chrono::duration_cast<nanosec>(clock::now().time_since_epoch()).count();
    embGenericGUID tval = hasher(std::this_thread::get_id());

    constexpr embGenericGUID halfSize = (sizeof(embGenericGUID) * CHAR_BIT) / 2;

    // replace lower half bits with thread ID in the rare case of 2 threads calling at same time.
    ret <<= halfSize;
    tval = (tval << halfSize) >> halfSize;
    return ret | tval;
}

EMB_NAMESPACE_END
