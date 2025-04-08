#pragma once

#include <cstdint> // for int usings
#include <limits> // for numeric_limits
#include <string> // for std::string and std::wstring
#include <string_view> // for std::string_view

constexpr float PI = 3.141592653f;
constexpr double PI_DBL = 3.14159265358979323846;
constexpr float INF_FLOAT = std::numeric_limits<float>::infinity();
constexpr double INF_DBL = std::numeric_limits<double>::infinity();
constexpr float NATURAL_LOG_OF_2 = 0.693147181f;
constexpr double NATURAL_LOG_OF_2_DBL = 0.693147181;
constexpr float SQR_ROOT_2 = 1.41421356237f;
constexpr double SQR_ROOT_2_DBL = 1.41421356237;

// Converts Radian to Degree units. (double)
static inline double RAD2DEG_DBL(double rad) { return rad * 180.0 / PI_DBL; }

// Converts Radian to Degree units. (float)
static inline float RAD2DEG(float rad) { return rad * 180.f / PI; }

// Converts Degree to Radian units. (double)
static inline double DEG2RAD_DBL(double deg) { return deg * PI_DBL / 180.0; }

// Converts Degree to Radian units. (float)
static inline float DEG2RAD(float deg) { return deg * PI / 180.f; }

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) (P)
#endif

#ifndef UNUSED_PARAM
#define UNUSED_PARAM(P) (P)
#endif

#ifndef UNUSED
#define UNUSED(P) (P)
#endif

// Primitive using renames. Extended from "vanilla" primitive types
using embBitmask8 = uint8_t; // 8-bit bitmask for bit operations
using embBitmask16 = uint16_t; // 16-bit bitmask for bit operations
using embBitmask32 = uint32_t; // 32-bit bitmask for bit operations
using embBitmas64 = uint64_t; // 64-bit bitmask for bit operations

using embChar = char; // UTF-8 char
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

// strings and stuff
using embStr = std::string;
using embWstr = std::wstring;
using embStrview = std::string_view;
