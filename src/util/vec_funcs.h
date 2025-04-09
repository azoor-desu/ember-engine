#pragma once

#include "defines.h"
#include "math.h"
#include "vec_base.h"
#include <cmath>

EMB_NAMESPACE_START

template<typename VecType>
VecType::value_type VecDot(const VecType& a, const VecType& b) noexcept
{
    typename VecType::value_type ret = 0;
    for (typename VecType::size_type i = 0; i < VecType::s_VecSize; i++)
    {
        ret += a[i] * b[i];
    }
    return ret;
}

template<typename VecType>
VecType::value_type VecMagnitudeSquare(const VecType& a) noexcept
{
    return VecDot(a, a);
}

template<typename VecType>
VecType::value_type VecMagnitude(const VecType& a) noexcept
{
    return sqrt(VecMagnitudeSquare(a));
}

template<typename VecType>
VecType::value_type VecDistanceSquare(const VecType& a, const VecType& b) noexcept
{
    return VecMagnitudeSquare(a - b);
}

template<typename VecType>
VecType::value_type VecDistance(const VecType& a, const VecType& b) noexcept
{
    return sqrt(VecDistanceSquare(a, b));
}

template<typename VecType>
VecType VecNormalize(const VecType& a) noexcept
{
    return a / VecMagnitude(a);
}

template<typename VecType>
VecType VecNormalizeFast(const VecType& a) noexcept
{
    return a * FastInvSqrt(VecMagnitudeSquare(a));
}

// For pre-compilation of common funcs
#define EXTERN_MACRO(vecType, valueType) \
    extern template valueType VecDot<vecType<valueType>>(const vecType<valueType>& a, \
                                                         const vecType<valueType>& b); \
    extern template valueType VecMagnitudeSquare<vecType<valueType>>(const vecType<valueType>& a); \
    extern template valueType VecMagnitude<vecType<valueType>>(const vecType<valueType>& a); \
    extern template valueType VecDistanceSquare<vecType<valueType>>(const vecType<valueType>& a, \
                                                                    const vecType<valueType>& b); \
    extern template valueType VecDistance<vecType<valueType>>(const vecType<valueType>& a, \
                                                              const vecType<valueType>& b); \
    extern template vecType<valueType> VecNormalize<vecType<valueType>>( \
        const vecType<valueType>& a); \
    extern template vecType<valueType> VecNormalizeFast<vecType<valueType>>( \
        const vecType<valueType>& a);

EXTERN_MACRO(embVec2, embF32)
EXTERN_MACRO(embVec3, embF32)
EXTERN_MACRO(embVec4, embF32)
#undef EXTERN_MACRO

// Vector specific

template<typename T>
embVec2<T> VecToVec2(const embVec3<T>& a) noexcept
{
    return embVec2<T>(a.x, a.y);
}

template<typename T>
embVec2<T> VecToVec2(const embVec4<T>& a) noexcept
{
    return embVec2<T>(a.x, a.y);
}

template<typename T>
embVec3<T> VecToVec3(const embVec2<T>& a) noexcept
{
    return embVec3<T>(a.x, a.y);
}

template<typename T>
embVec3<T> VecToVec3(const embVec4<T>& a) noexcept
{
    return embVec3<T>(a.x, a.y, a.z);
}

template<typename T>
embVec4<T> VecToVec4(const embVec2<T>& a) noexcept
{
    return embVec4<T>(a.x, a.y);
}

template<typename T>
embVec4<T> VecToVec4(const embVec3<T>& a) noexcept
{
    return embVec4<T>(a.x, a.y, a.z);
}



EMB_NAMESPACE_END
