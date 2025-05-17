#pragma once

// Namespace shorthand
#define EMB_NAMESPACE_START \
    namespace ember \
    {
#define EMB_NAMESPACE_END } // namespace ember

// TODO: Make this thread safe-r.
// Shorthand for singleton getter member functions
#define EMB_CLASS_SINGLETON_MACRO(className) \
    static className& Instance() \
    { \
        static className obj; \
        return obj; \
    }
