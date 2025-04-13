#pragma once

#include "vec.h"
#include <format>

// Print format helpers
template<typename T>
struct std::formatter<ember::embVec<2, T>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    auto format(const ember::embVec<2, T>& val, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[{}, {}]", val[0], val[1]);
    }
};

template<typename T>
struct std::formatter<ember::embVec<3, T>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    auto format(const ember::embVec<3, T>& val, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[{}, {}, {}]", val[0], val[1], val[2]);
    }
};

template<typename T>
struct std::formatter<ember::embVec<4, T>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    auto format(const ember::embVec<4, T>& val, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[{}, {}, {}]", val[0], val[1], val[2], val[3]);
    }
};

// compile optim
// https://arne-mertz.de/2019/02/extern-template-reduce-compile-times/
#define EMB_DEFINE_VEC_UTIL(size, valueType) \
    template class std::formatter<ember::embVec<size, ember::valueType>>;

#define EMB_DEFINE_VEC_UTIL_ALL(valueType) \
    EMB_DEFINE_VEC_UTIL(2, valueType) \
    EMB_DEFINE_VEC_UTIL(3, valueType) \
    EMB_DEFINE_VEC_UTIL(4, valueType)

#define EMB_DECLARE_VEC_UTIL(size, valueType) extern EMB_DEFINE_VEC_UTIL(size, valueType)

#define EMB_DECLARE_VEC_UTIL_ALL(valueType) \
    EMB_DECLARE_VEC_UTIL(2, valueType) \
    EMB_DECLARE_VEC_UTIL(3, valueType) \
    EMB_DECLARE_VEC_UTIL(4, valueType)

EMB_DEFINE_VEC_UTIL(2, embF32)
// EMB_DECLARE_VEC_UTIL_ALL(embF32)
// EMB_DECLARE_VEC_UTIL_ALL(embS32)
// EMB_DECLARE_VEC_UTIL_ALL(embU32)
