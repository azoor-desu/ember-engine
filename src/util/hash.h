#pragma once

#include "macros.h"
#include "str.h"
#include "types.h"

EMB_NAMESPACE_START

// Generic GUID type. Specialize this into further GUID types if needed.
using embGenericGuid = embU64;

class Hash
{
    Hash() = delete;
    ~Hash() = delete;

  public:
    // Basic FNV-1a Hashing: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
    // Not the best but super simple.
    consteval static embGenericGuid GenerateHash(embStrView strView) noexcept
    {
        if (strView.empty())
            return 0;

        embGenericGuid result = 0xcb'f2'9c'e4'84'22'23'25;
        for (embU8 c : strView)
        {
            result ^= c;
            result *= 0x00'00'01'00'00'00'01'b3;
        }
        return result << 1;
    }

    static embGenericGuid GenerateGUID() noexcept;
};

EMB_NAMESPACE_END
