#pragma once

#include "macros.h"
#include "str.h"
#include "types.h"

EMB_NAMESPACE_START

class Hash
{
    Hash() = delete;
    ~Hash() = delete;

  public:
    consteval static embHash GenerateHash(embStrView strView) noexcept
    {
        return (embHash)GenerateHash64(strView);
    }

    // Basic FNV-1a Hashing: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
    // Not the best but super simple.
    consteval static embHash64 GenerateHash64(embStrView strView) noexcept
    {
        if (strView.empty())
            return 0;

        embHash64 result = 0xcb'f2'9c'e4'84'22'23'25;
        for (embU8 c : strView)
        {
            result ^= c;
            result *= 0x00'00'01'00'00'00'01'b3;
        }
        return result << 1;
    }

    // Uses dumb algo to spit out a unique enough 32/64bit GUID.
    static embGuid GenerateGUID() noexcept;
    static embGuid64 GenerateGUID64() noexcept;

    // Computes a unique hash of the type passed as the template parameter.
    // Will always generate the same hash for the same class, but may not be consistent across platforms or compilers.
    // Passing in additional qualifiers e.g. <const T> vs <T> will result in a different hash.
    // The permutation does not matter e.g. <T const> == <const T>
    template <typename T>
    consteval static embHash64 GetTypeHash64() noexcept
    {
#ifdef _MSC_VER
        // MSVC version of getting function signature of this function
        return GenerateHash64(__FUNCSIG__);
#else
        // Non-MSVC version of getting function signature of this function
        return GenerateHash64(__PRETTY_FUNCTION__);
#endif
    }

    // Computes a unique hash of the type passed as the template parameter.
    // Will always generate the same hash for the same class, but may not be consistent across platforms or compilers.
    // Passing in additional qualifiers e.g. <const T> vs <T> will result in a different hash.
    // The permutation does not matter e.g. <T const> == <const T>
    template <typename T>
    consteval static embHash GetTypeHash() noexcept
    {
#ifdef _MSC_VER
        // MSVC version of getting function signature of this function
        return GenerateHash(__FUNCSIG__);
#else
        // Non-MSVC version of getting function signature of this function
        return GenerateHash(__PRETTY_FUNCTION__);
#endif
    }
};

EMB_NAMESPACE_END
