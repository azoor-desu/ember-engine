#pragma once

#include "types.h"
#include <cmath>
#include <cstdint>

EMB_NAMESPACE_START

constexpr embF32 embPI = 3.141592653f;
constexpr embF64 embPI_F64 = 3.14159265358979323846;
constexpr embF32 embLn2 = 0.693147181f;
constexpr embF64 embLn2_F64 = 0.693147181;
constexpr embF32 embSqrt2 = 1.41421356237f;
constexpr embF64 embSqrt2_F64 = 1.41421356237;

[[nodiscard]] constexpr embF64 RAD2DEG(embF64 rad) { return rad * 180.0 / embPI_F64; }
[[nodiscard]] constexpr embF32 RAD2DEG(embF32 rad) { return rad * 180.f / embPI; }
[[nodiscard]] constexpr embF64 DEG2RAD(embF64 deg) { return deg * embPI_F64 / 180.0; }
[[nodiscard]] constexpr embF32 DEG2RAD(embF32 deg) { return deg * embPI / 180.f; }

// Helpers

/// <summary>
/// Compares 2 values to see if they are almost equal\n
/// "Unit of least precision" means the smallest possible unit able to be represented.\n
/// If set to 0, then values need to be exactly equal.\n
///	If set to 1, then values that are 1 epsilon apart are considered almost equal.
/// https://stackoverflow.com/a/65015333
/// </summary>
/// <param name="x">first value to compare.</param>
/// <param name="y">second value to compare.</param>
/// <param name="ulp">units of least presicion.</param>
/// <returns>Returns if 2 floating point type values are almost equal or not,
/// with specified accuracy of ulp steps.</returns>
template<class T>
[[nodiscard]]
constexpr inline typename std::enable_if<!std::numeric_limits<T>::is_integer, embBool>::type
AlmostEqual(const T& x, const T& y, int ulp = 2)
{

    // Do the trivial comparison if available.
    if (x == y)
        return true;

    // If either one inf but not the other, return false.
    if ((std::isinf(x) || std::isinf(y)) && (!std::isinf(x) || !std::isinf(y)))
        return false;

    // If both inf, just compare if both are +ve or -ve.
    if (std::isinf(x) && std::isinf(y))
        return ((x > 0 && y > 0) || (x < 0 && y < 0));

    // Smallest difference in floats between 1 and the next number, vs 5000 and the next number, is different.
    // Scale epsilon to emulate this behaviour.
    // Smallest scale shld be 1 (don't go below epsilon), largest scale is the largest number.
    T scale = std::max({(T)1.0, std::abs(x), std::abs(y)});

    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::abs(x - y) <= std::numeric_limits<T>::epsilon() * scale * (T)ulp
           // unless the result is subnormal
           || std::abs(x - y) < std::numeric_limits<T>::min();
}

// References: https://en.wikipedia.org/wiki/Fast_inverse_square_root
/// Returns an approximation of 1/sqrt(value).
[[nodiscard]]
constexpr embF32 FastInvSqrt(embF32 value, embU32 iterations)
{
    // Enable only on IEEE 754
    static_assert(std::numeric_limits<embF32>::is_iec559);

    // Evil magic constant. Don't try to understand.
    // Magic value - embF32 bit-shifted by 1 as an int = approximate 1/sqrt value.
    embF32 y = std::bit_cast<embF32>(0x5F'37'5A'86 - (std::bit_cast<std::uint32_t>(value) >> 1));

    // newtons method
    for (embU32 i = 0; i < iterations; ++i)
    {
        y = y * (1.5f - (value * 0.5f * y * y));
    }

    return y;
}

// Returns an approximation of 1/sqrt(value).
[[nodiscard]]
constexpr embF32 FastInvSqrt(embF32 value)
{
    return FastInvSqrt(value, 3);
}

// Returns an approximation of 1/sqrt(value).
[[nodiscard]]
constexpr embF64 FastInvSqrt(embF64 value, embU32 iterations)
{
    // Enable only on IEEE 754, else stop program
    static_assert(std::numeric_limits<embF32>::is_iec559);

    // Evil magic constant. Don't try to understand.
    // Magic value - embF32 bit-shifted by 1 as an int = approximate 1/sqrt value.
    embF64 y = std::bit_cast<embF64>(0x5F'E6'EB'50'C7'B5'37'A9
                                     - (std::bit_cast<std::uint64_t>(value) >> 1));

    // newtons method
    for (embU32 i = 0; i < iterations; ++i)
    {
        y = y * (1.5 - (value * 0.5 * y * y));
    }

    return y;
}

// Returns an approximation of 1/sqrt(value).
[[nodiscard]]
constexpr embF64 FastInvSqrt(embF64 value)
{
    return FastInvSqrt(value, 3);
}

EMB_NAMESPACE_END
