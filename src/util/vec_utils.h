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

    auto format(const ember::embVec3<T>& val, std::format_context& ctx) const
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

    auto format(const ember::embVec4<T>& val, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[{}, {}, {}, {}]", val.x, val.y, val.z, val.w);
    }
};

// For pre-compilation of common classes
#define EXTERN_MACRO(valueType) \
    extern template class std::formatter<ember::embVec2<ember::valueType>>; \
    extern template class std::formatter<ember::embVec3<ember::valueType>>; \
    extern template class std::formatter<ember::embVec4<ember::valueType>>;

EXTERN_MACRO(embF32)
// EXTERN_MACRO(embF64)
// EXTERN_MACRO(embU8)
// EXTERN_MACRO(embS32)
// EXTERN_MACRO(embU32)

#undef EXTERN_MACRO
