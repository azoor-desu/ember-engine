#pragma once

// ===== Namespace ====
#define EMB_NAMESPACE_START \
    namespace ember \
    {
#define EMB_NAMESPACE_END } // namespace ember

// ===== Singleton creator ====
// TODO: Make this thread safe-r.
// Shorthand for singleton getter member functions
#define EMB_CLASS_SINGLETON_MACRO(className) \
    static className& Instance() \
    { \
        static className singletonClass; \
        return singletonClass; \
    }

// ===== Misc Macro Helpers ====
#ifdef EMB_DEF_DEBUG
#    define EMB_MACRO_START \
        do \
        {
#    define EMB_MACRO_END \
        } \
        while (0)
#else
// Slightly more optimal(?) macro. Not sure if true.
#    define EMB_MACRO_START \
        if (1) \
        {
#    define EMB_MACRO_END \
        } \
        else \
        {}
#endif


// https://stackoverflow.com/a/48820063

#define DECLARE_ENUM_WITH_TYPE(className, typeName, ...) \
    enum class className : typeName \
    { \
        __VA_ARGS__ \
    }; \
    std::unordered_map<typeName, std::string> className##MapName(generateEnumMap<typeName>(#__VA_ARGS__));

#define DECLARE_ENUM(className, ...) DECLARE_ENUM_WITH_TYPE(className, int32_t, __VA_ARGS__)
