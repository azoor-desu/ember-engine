#pragma once

#include "defines.h"
#include <bitset> // for bitmask
#include <cstddef>
#include <cstdint> // for int types
#include <limits> // for numeric_limits

EMB_NAMESPACE_START

// boolean type
using embBool = bool;
// UTF-8 char
using embChar = char;
// UTF-16 char
using embLChar = char16_t;
// UTF-16/32, system preferred. usually 16 on other platforms, 32 on Linux.
using embWChar = wchar_t;

// TODO: CUSTOM CLASS FOR BITMASK, OR USE STD BITSET.
template<int T>
using embBitmask = std::bitset<T>;
// 8-bit bitmask for bit operations
using embBitmask8 = std::bitset<8>;
// 16-bit bitmask for bit operations
using embBitmask16 = std::bitset<16>;
// 32-bit bitmask for bit operations
using embBitmask32 = std::bitset<32>;
// 64-bit bitmask for bit operations
using embBitmask64 = std::bitset<64>;

// Size of memory address space, usually 64bit.
using embSizeType = std::size_t;
// Generic pointer type. Do not use directly, cast to something else first.
using embPtr = embSizeType*;

// 8-bits, 1 byte, signed
using embS8 = int8_t;
// 8-bits, 1 byte, unsigned
using embU8 = uint8_t;
// 16-bits, 2 bytes, signed
using embS16 = int16_t;
// 16-bits, 2 bytes, unsigned
using embU16 = uint16_t;
// 32-bits, 4 bytes, signed
using embS32 = int32_t;
// 32-bits, 4 bytes, unsigned
using embU32 = uint32_t;
// 64-bits, 8 bytes, signed
using embS64 = int64_t;
// 64-bits, 8 bytes, unsigned
using embU64 = uint64_t;

// 32-bits, 4 bytes
using embF32 = float;
// 64-bits, 8 bytes
using embF64 = double;

constexpr embS8 embS8_MAX = std::numeric_limits<embS8>::max();
constexpr embS8 embS8_MIN = std::numeric_limits<embS8>::min();
constexpr embU8 embU8_MAX = std::numeric_limits<embU8>::max();
constexpr embU8 embU8_MIN = 0;
constexpr embS16 embS16_MAX = std::numeric_limits<embS16>::max();
constexpr embS16 embS16_MIN = std::numeric_limits<embS16>::min();
constexpr embU16 embU16_MAX = std::numeric_limits<embU16>::max();
constexpr embU16 embU16_MIN = 0;
constexpr embS32 embS32_MAX = std::numeric_limits<embS32>::max();
constexpr embS32 embS32_MIN = std::numeric_limits<embS32>::min();
constexpr embU32 embU32_MAX = std::numeric_limits<embU32>::max();
constexpr embU32 embU32_MIN = 0;
constexpr embS64 embS64_MAX = std::numeric_limits<embS64>::max();
constexpr embS64 embS64_MIN = std::numeric_limits<embS64>::min();
constexpr embU64 embU64_MAX = std::numeric_limits<embU64>::max();
constexpr embU64 embU64_MIN = 0;

constexpr float embF32_MAX = std::numeric_limits<float>::max();
constexpr double embF64_MIN = std::numeric_limits<double>::min();
constexpr float embF32_INF = std::numeric_limits<float>::infinity();
constexpr double embF64_INF = std::numeric_limits<double>::infinity();
// the difference between 1 and the smallest floating point number greater than 1
constexpr double embF32_EPSILON = 1.192092896e-07F;
// the difference between 1 and the smallest floating point number greater than 1
constexpr double embF64_EPSILON = 2.2204460492503131e-016;
// smallest possible positive value greater than 0
constexpr double embF32_SMALLEST = 1.175494351E-38F;
// smallest possible positive value greater than 0
constexpr double embF64_SMALLEST = 2.2250738585072014E-308;

EMB_NAMESPACE_END
