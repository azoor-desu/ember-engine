#pragma once

#include "types.h"
#include <cmath> // for inf

EMB_NAMESPACE_START

constexpr embF32 embPI = 3.141592653f;
constexpr embF64 embPI_F64 = 3.14159265358979323846;
constexpr embF32 embLn2 = 0.693147181f;
constexpr embF64 embLn2_F64 = 0.693147181;
constexpr embF32 embSqrt2 = 1.41421356237f;
constexpr embF64 embSqrt2_F64 = 1.41421356237;

[[nodiscard]] constexpr embF64 RadianToDegree(embF64 rad) { return rad * 180.0 / embPI_F64; }
[[nodiscard]] constexpr embF32 RadianToDegree(embF32 rad) { return rad * 180.f / embPI; }
[[nodiscard]] constexpr embF64 DegreeToRadian(embF64 deg) { return deg * embPI_F64 / 180.0; }
[[nodiscard]] constexpr embF32 DegreeToRadian(embF32 deg) { return deg * embPI / 180.f; }

// Not complete. Trivial but more lightweight implementation. Full implementation:
// https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
[[nodiscard]]
inline bool AlmostEqual(const embF32 x, const embF32 y)
{
    if (x == y)
        return true;

    // cannot compare inf and nan cases.
    if (std::isinf(x) || std::isinf(y) || std::isnan(x) || std::isnan(y))
        return false;

    // Use epsilon to compare. But epsilon needs to be scaled up if values are big...
    embF32 eps = embF32_EPSILON * std::max(1.f, std::max(std::abs(x), std::abs(y)));
    return std::abs(x - y) < eps;
}

// Not complete. Trivial but more lightweight implementation. Full implementation:
// https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
[[nodiscard]]
inline bool AlmostEqual(const embF64 x, const embF64 y)
{
    if (x == y)
        return true;

    // cannot compare inf cases.
    if ((std::isinf(x) || std::isinf(y)))
        return false;

    // Use epsilon to compare. But epsilon needs to be scaled up if values are big...
    embF64 eps = embF64_EPSILON * std::max(1.0, std::max(std::abs(x), std::abs(y)));
    return std::abs(x - y) < eps;
}

// References: https://en.wikipedia.org/wiki/Fast_inverse_square_root
// Returns an approximation of 1/sqrt(value).
[[nodiscard]]
inline embF32 FastInvSqrt(embF32 value)
{
    // Enable only on IEEE 754
    static_assert(std::numeric_limits<embF32>::is_iec559);

    // Evil magic constant. Don't try to understand.
    // Magic value - embF32 bit-shifted by 1 as an int = approximate 1/sqrt value.
    embF32 y = std::bit_cast<embF32>(0x5F'37'5A'86 - (std::bit_cast<std::uint32_t>(value) >> 1));

    // newtons method
    for (embU32 i = 0; i < 3; ++i)
    {
        y = y * (1.5f - (value * 0.5f * y * y));
    }

    return y;
}

// Returns an approximation of 1/sqrt(value).
[[nodiscard]]
inline embF64 FastInvSqrt(embF64 value)
{
    // Enable only on IEEE 754, else stop program
    static_assert(std::numeric_limits<embF32>::is_iec559);

    // Evil magic constant. Don't try to understand.
    // Magic value - embF32 bit-shifted by 1 as an int = approximate 1/sqrt value.
    embF64 y = std::bit_cast<embF64>(0x5F'E6'EB'50'C7'B5'37'A9
                                     - (std::bit_cast<std::uint64_t>(value) >> 1));

    // newtons method
    for (embU32 i = 0; i < 3; ++i)
    {
        y = y * (1.5 - (value * 0.5 * y * y));
    }

    return y;
}

EMB_NAMESPACE_END
