#pragma once
#include "util/macros.h"
#include <cassert>
#include <csignal>

EMB_NAMESPACE_START

// ===== Toggles =====
// debug levels
#if defined(_DEBUG) || !defined(NDEBUG) || defined(DEBUG)
#    define EMB_DEF_DEBUG
#else
#    define EMB_DEF_RELEASE
#endif

// OS
// https://sourceforge.net/p/predef/wiki/OperatingSystems/
#if defined(__linux__)
#    define EMB_DEF_LINUX
#elif defined(_WIN32) /* || defined(__CYGWIN__) */
#    define EMB_DEF_WINDOWS
#else
#    error "Operating system not supported!"
#endif

// Compiler
// https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html
#if defined(__clang__)
#    define EMB_DEF_CLANG
#elif defined(__GNUC__)
#    define EMB_DEF_GCC
#elif defined(_MSC_VER)
#    define EMB_DEF_MSVC
#else
#    error "Compiler not supported!"
#endif

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

// ===== Compiler Hints ====
#if defined(EMB_DEF_CLANG) || defined(EMB_DEF_GCC)
#    define EMB_UNLIKELY(IFEXPR) __builtin_expect(bool(IFEXPR), 0)
#    define EMB_LIKELY(IFEXPR) __builtin_expect(bool(IFEXPR), 1)

#    define EMB_ALWAYS_INLINE __attribute((always_inline))
#    define EMB_STRONG_INLINE inline
#    define EMB_NO_INLINE __attribute__((noinline))

#    define EMB_DEPRECATED __attribute__((deprecated))

#    define EMB_DLL_EXPORT
#    define EMB_DLL_IMPORT

#    define EMB_STDCALL __attribute__((stdcall))
#    define EMB_CDECL /* default */
#    define EMB_FASTCALL __attribute__((fastcall))

#    define EMB_THREAD_LOCAL thread_local // May not work for gcc?
#elif defined(EMB_DEF_MSVC)
#    undef CDECL
// Unfortunately visual studio does not have a branch prediction primitive.
#    define EMB_UNLIKELY(IFEXPR) IFEXPR
#    define EMB_LIKELY(IFEXPR) IFEXPR

#    define EMB_ALWAYS_INLINE __forceinline
#    define EMB_STRONG_INLINE __forceinline
#    define EMB_NO_INLINE __declspec(noinline)

#    define EMB_DEPRECATED __declspec(deprecated)

#    define EMB_DLL_EXPORT __declspec(dllexport)
#    define EMB_DLL_IMPORT __declspec(dllimport)

#    define EMB_STDCALL __stdcall
#    define EMB_CDECL __cdecl
#    define EMB_FASTCALL __fastcall

#    define EMB_THREAD_LOCAL __declspec(thread)
#else
#    error "unsupported platform for compiler hints"
#endif

#if defined(EMB_DEF_CLANG)
#    define EMB_OPTIMIZE_OFF _Pragma("clang optimize off")
#    define EMB_OPTIMIZE_ON _Pragma("clang optimize on")
#elif defined(EMB_DEF_GCC)
// Might be wrong
#    define EMB_OPTIMIZE_OFF _Pragma("GCC optimize(\"O0\")")
#    define EMB_OPTIMIZE_ON _Pragma("GCC optimize(\"O3\")")
#elif defined(EMB_DEF_MSVC)
#    define EMB_OPTIMIZE_OFF __pragma(optimize("", off))
#    define EMB_OPTIMIZE_ON __pragma(optimize("", on))
#endif

// ===== Breakpoint ====
#if defined(EMB_DEF_CLANG) || defined(EMB_DEF_GCC)
#    define EMB_BREAKPOINT() std::raise(SIGINT)
#elif defined(EMB_DEF_MSVC)
#    define EMB_BREAKPOINT() __debugbreak()
#else
#    define EMB_BREAKPOINT() // don't break.
#endif

// ===== Logs =====
// TODO replace with spdlog backend
#define EMB_LOG_TRACE(msg, ...) printf(msg);
#define EMB_LOG_DEBUG(msg, ...) printf(msg);
#define EMB_LOG_INFO(msg, ...) printf(msg);
#define EMB_LOG_WARN(msg, ...) printf(msg);
#define EMB_LOG_ERROR(msg, ...) printf(msg);

#define EMB_LOG_WARN_IF(expr, msg, ...)
#define EMB_LOG_ERROR_IF(expr, msg, ...)

// ===== Asserts =====
// TODO
#define EMB_ASSERT_STATIC(expr, msg, ...) static_assert((expr), (msg))
#define EMB_ASSERT_HARD(expr, msg, ...) assert((expr))
#define EMB_ASSERT_SOFT(expr, msg, ...) assert((expr))

// ===== Short logic toggles =====
#ifdef EMB_DEF_DEBUG
#    define EMB_IFDEF_DEBUG (code) code
#else
#    define EMB_IFDEF_DEBUG (code)
#endif
#ifdef EMB_DEF_RELEASE
#    define EMB_IFDEF_RELEASE (code) code
#else
#    define EMB_IFDEF_RELEASE (code)
#endif

EMB_NAMESPACE_END
