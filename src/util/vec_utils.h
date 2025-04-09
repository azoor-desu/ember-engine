#pragma once

#include "vec_base.h"
#include <format>

// Print format helpers
template<typename T>
struct std::formatter<ember::embVec2<T>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    auto format(const ember::embVec2<T>& val, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[{}, {}]", val.x, val.y);
    }
};

template<typename T>
struct std::formatter<ember::embVec3<T>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    auto format(const ember::embVec2<T>& val, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[{}, {}, {}]", val.x, val.y, val.z);
    }
};

template<typename T>
struct std::formatter<ember::embVec4<T>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    auto format(const ember::embVec2<T>& val, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[{}, {}, {}, {}]", val.x, val.y), val.z, val.w;
    }
};
