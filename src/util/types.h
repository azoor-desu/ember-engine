#pragma once

#include "defines.h"
#include <cstdint> // for int types
#include <limits> // for numeric_limits

EMB_NAMESPACE_START

using embBool = bool;

using embBitmask8 = uint8_t; // 8-bit bitmask for bit operations
using embBitmask16 = uint16_t; // 16-bit bitmask for bit operations
using embBitmask32 = uint32_t; // 32-bit bitmask for bit operations
using embBitmask64 = uint64_t; // 64-bit bitmask for bit operations

using embChar = char; // UTF-8 char
//using embLChar = lchar; // UTF-16
using embWChar = wchar_t; // UTF-16

using embS8 = int8_t; // 8-bits, 1 byte, signed
using embU8 = uint8_t; // 8-bits, 1 byte, unsigned
using embS16 = int16_t; // 16-bits, 2 bytes, signed
using embU16 = uint16_t; // 16-bits, 2 bytes, unsigned
using embS32 = int32_t; // 32-bits, 4 bytes, signed
using embU32 = uint32_t; // 32-bits, 4 bytes, unsigned
using embS64 = int64_t; // 64-bits, 8 bytes, signed
using embU64 = uint64_t; // 64-bits, 8 bytes, unsigned

using embF32 = float; // 32-bits, 4 bytes
using embF64 = double; // 64-bits, 8 bytes

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

EMB_NAMESPACE_END
