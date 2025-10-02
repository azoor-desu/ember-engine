#pragma once
#include <cstdarg>
#include <unordered_map>

#include "macros.h"
#include "str.h"
#include "types.h"

EMB_NAMESPACE_START

inline const std::unordered_map<int, embStr> CreateEnumStringMap(int count, ...)
{
    std::unordered_map<int, std::string> ret {};

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++)
    {
        std::string str = va_arg(args, const char*);
        ret.insert({i, str});
    }

    va_end(args);
    return ret;
}

// ========== X-macro Functions ==========
#define EMB_X_ENUM_VAL(className, valName) valName,
#define EMB_X_ENUM_STR_CONVERSION(className, valName) \
    case className::valName: \
        return #valName;

// Usage:
// #define RESOURCE_TYPE_LIST(X) \
//     X(FooBar, ONE) \
//     X(FooBar, TWO) \
//     X(FooBar, THREE)
// enum class ResourceType : embU8
// {
//     RESOURCE_TYPE_LIST(EMB_X_ENUM_VAL)
//         ENUM_COUNT
// };
// inline embStrView EnumToStr(ResourceType val)
// {
//     switch (val)
//     {
//         RESOURCE_TYPE_LIST(EMB_X_ENUM_STR_CONVERSION)
//     }
//     std::unreachable();
// }
// #undef RESOURCE_TYPE_LIST


EMB_NAMESPACE_END
