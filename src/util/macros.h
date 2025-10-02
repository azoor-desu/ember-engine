#pragma once

// ===== Misc ====
#ifndef TRUE
#    define TRUE 1
#endif
#ifndef FALSE
#    define FALSE 0
#endif
#ifndef EMB_UNUSED_PARAM
#    define EMB_UNUSED_PARAM(P) (P)
#endif

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
