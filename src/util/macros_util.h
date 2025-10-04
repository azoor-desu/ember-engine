#pragma once
#include <cstdarg>
#include <unordered_map>

#include "hash.h"
#include "macros.h"
#include "macros_debug.h"
#include "str.h"
#include "types.h"

EMB_NAMESPACE_START

// ========== X-macro Functions ==========

// List populator helpers
#define _EMB_X_INSERT_ENUM_VAL(enumClassName, valName) valName,

#define _EMB_X_INSERT_ENUM_SWITCH_STR(enumClassName, valName) \
    case enumClassName::valName: \
        return #valName;

#define _EMB_X_INSERT_ENUM_HASH_HELPER(classValName) \
    case classValName: \
        return Hash::GenerateHash(#classValName);

#define _EMB_X_INSERT_ENUM_HASH_SWITCH_STR(enumClassName, valName) \
    _EMB_X_INSERT_ENUM_HASH_HELPER(enumClassName::valName)

#define _EMB_X_INSERT_ENUM_FROMHASH_HELPER(classValName) \
    case Hash::GenerateHash(#classValName): \
        return classValName;

#define _EMB_X_INSERT_ENUM_FROMHASH_SWITCH_STR(enumClassName, valName) \
    _EMB_X_INSERT_ENUM_FROMHASH_HELPER(enumClassName::valName)

// Builder: Enum
#define EMB_X_DEF_ENUM(enumClassName, enumSizeType, TYPE_LIST) \
    enum class enumClassName : enumSizeType \
    { \
        TYPE_LIST(_EMB_X_INSERT_ENUM_VAL) \
            ENUM_COUNT \
    };

// Builder: Enum -> Str
#define EMB_X_DEF_ENUM_TO_STR(enumClassName, TYPE_LIST) \
    inline constexpr embStrView Enum##enumClassName##ToStr(enumClassName val) \
    { \
        switch (val) \
        { \
            TYPE_LIST(_EMB_X_INSERT_ENUM_SWITCH_STR) \
        default: \
            EMB_ASSERT_HARD(false, "Invalid value provided"); \
            break; \
        } \
        EMB_PATH_UNREACHABLE \
    }

// Builder: Enum -> Hash/GUID
#define EMB_X_DEF_ENUM_TO_HASH(enumClassName, TYPE_LIST) \
    inline constexpr embHash EnumResourceTypeToHash(ResourceType val) \
    { \
        switch (val) \
        { \
            TYPE_LIST(_EMB_X_INSERT_ENUM_HASH_SWITCH_STR) \
        default: \
            EMB_ASSERT_HARD(false, "Invalid value provided"); \
            break; \
        } \
        EMB_PATH_UNREACHABLE \
    }

// Builder: Hash/GUID -> Enum
#define EMB_X_DEF_ENUM_FROM_HASH(enumClassName, TYPE_LIST) \
    inline constexpr ResourceType EnumResourceTypeFromHash(embHash val) \
    { \
        switch (val) \
        { \
            TYPE_LIST(_EMB_X_INSERT_ENUM_FROMHASH_SWITCH_STR) \
        default: \
            EMB_ASSERT_HARD(false, "Invalid value provided"); \
            break; \
        } \
        EMB_PATH_UNREACHABLE \
    }

// Getters
#define EMB_X_ENUM_TO_STR(enumClassName, enumVal) Enum##enumClassName##ToHash(enumVal)
#define EMB_X_ENUM_TO_HASH(enumClassName, enumVal) Enum##enumClassName##ToHash(enumVal)
#define EMB_X_ENUM_FROM_HASH(enumClassName, hashVal) Enum##enumClassName##FromHash(hashVal)

EMB_NAMESPACE_END
