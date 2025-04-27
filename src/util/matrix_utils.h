#pragma once

#include "matrix.h"
#include <format>

// Print format helpers
template<typename T>
struct std::formatter<ember::embMtx<2, T>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    auto format(const ember::embMtx<2, T>& val, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "\n| {:<9.3} {:<9.3} |\n| {:<9.3} {:<9.3} |", val[0][0],
                              val[1][0], val[0][1], val[1][1]);
    }
};

template<typename T>
struct std::formatter<ember::embMtx<3, T>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    auto format(const ember::embMtx<3, T>& val, std::format_context& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "\n| {:<9.3} {:<9.3} {:<9.3} |\n| {:<9.3} {:<9.3} {:<9.3} |\n| {:<9.3} {:<9.3} {:<9.3} |",
            val[0][0], val[1][0], val[2][0], val[0][1], val[1][1], val[2][1], val[0][2], val[1][2],
            val[2][2]);
    }
};

template<typename T>
struct std::formatter<ember::embMtx<4, T>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    auto format(const ember::embMtx<4, T>& val, std::format_context& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "\n| {:<9.3} {:<9.3} {:<9.3} {:<9.3} |\n| {:<9.3} {:<9.3} {:<9.3} {:<9.3} |\n| {:<9.3} {:<9.3} {:<9.3} {:<9.3} |\n| {:<9.3} {:<9.3} {:<9.3} {:<9.3} |",
            val[0][0], val[1][0], val[2][0], val[3][0], val[0][1], val[1][1], val[2][1], val[3][1],
            val[0][2], val[1][2], val[2][2], val[3][2], val[0][3], val[1][3], val[2][3], val[3][3]);
    }
};

// compile optim
// https://arne-mertz.de/2019/02/extern-template-reduce-compile-times/
#define EMB_DEFINE_MTX_UTIL(size, valueType) \
    template class std::formatter<ember::embMtx<size, ember::valueType>>;

#define EMB_DEFINE_MTX_UTIL_ALL(valueType) \
    EMB_DEFINE_MTX_UTIL(2, valueType) \
    EMB_DEFINE_MTX_UTIL(3, valueType) \
    EMB_DEFINE_MTX_UTIL(4, valueType)

#define EMB_DECLARE_MTX_UTIL(size, valueType) extern EMB_DEFINE_MTX_UTIL(size, valueType)

#define EMB_DECLARE_MTX_UTIL_ALL(valueType) \
    EMB_DECLARE_MTX_UTIL(2, valueType) \
    EMB_DECLARE_MTX_UTIL(3, valueType) \
    EMB_DECLARE_MTX_UTIL(4, valueType)

EMB_DECLARE_MTX_UTIL_ALL(embF32)
EMB_DECLARE_MTX_UTIL_ALL(embF64)
// EMB_DECLARE_MTX_UTIL_ALL(embS32)
// EMB_DECLARE_MTX_UTIL_ALL(embU32)
