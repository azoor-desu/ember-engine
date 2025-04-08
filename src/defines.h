#pragma once

#include <cstdint> // for int usings
#include <limits> // for numeric_limits
#include <string> // for std::string and std::wstring
#include <string_view> // for std::string_view

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

constexpr float embPI = 3.141592653f;
constexpr double embPI_F64 = 3.14159265358979323846;
constexpr float embLn2 = 0.693147181f;
constexpr double embLn2_F64 = 0.693147181;
constexpr float embSqrt2 = 1.41421356237f;
constexpr double embSqrt2_F64 = 1.41421356237;

constexpr double RAD2DEG(double rad) { return rad * 180.0 / embPI_F64; }
constexpr float RAD2DEG(float rad) { return rad * 180.f / embPI; }
constexpr double DEG2RAD(double deg) { return deg * embPI_F64 / 180.0; }
constexpr float DEG2RAD(float deg) { return deg * embPI / 180.f; }

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
constexpr float embF32_INF = std::numeric_limits<float>::infinity();
constexpr double embF64_INF = std::numeric_limits<double>::infinity();

// strings and stuff
using embStr = std::string;
using embWstr = std::wstring;
using embStrview = std::string_view;
