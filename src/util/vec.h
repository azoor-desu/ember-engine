#pragma once

#include "types.h"

EMB_NAMESPACE_START

// usual containers

// vec2/3/4 etc

template<embU32 N, typename T = embF32>
class embVec {
  public:
    T m_Data[N];

    // Constructor
    embVec<N, T>() noexcept = default;

    // Standard ops
    constexpr bool operator==(const embVec<N, T>& rhs) const noexcept = default;

    embVec<N, T>& operator=(const embVec<N, T>& rhs) noexcept
    {
        for (decltype(N) i = 0; i < N; i++)
            this->m_Data[i] = rhs.m_Data[i];
        return *this;
    }

    [[nodiscard]]
    embVec<N, T> operator+(const embVec<N, T>& rhs) const noexcept
    {
        embVec<N, T> ret = *this;
        for (decltype(N) i = 0; i < N; i++)
            ret.m_Data[i] += rhs.m_Data[i];

        return ret;
    }

    embVec<N, T>& operator+=(const embVec<N, T>& rhs) noexcept
    {
        *this = *this + rhs;
        return *this;
    }

    [[nodiscard]]
    embVec<N, T> operator-(const embVec<N, T>& rhs) const noexcept
    {
        embVec<N, T> ret = *this;
        for (decltype(N) i = 0; i < N; i++)
            ret.m_Data[i] -= rhs.m_Data[i];

        return ret;
    }

    embVec<N, T>& operator-=(const embVec<N, T>& rhs) const noexcept
    {
        *this = *this - rhs;
        return *this;
    }

    T operator[](decltype(N) i) const { return m_Data[i]; }
    T& operator[](decltype(N) i) { return m_Data[i]; }
};

using embVec2 = embVec<2, embF32>;
using embVec3 = embVec<3, embF32>;
using embVec4 = embVec<4, embF32>;

using embVec2I = embVec<2, embS32>;
using embVec3I = embVec<3, embS32>;
using embVec4I = embVec<4, embS32>;
using embVec2U = embVec<2, embU32>;
using embVec3U = embVec<3, embU32>;
using embVec4U = embVec<4, embU32>;

// array containers

EMB_NAMESPACE_END

// TODO: Implement sth like glm, don't do vec.cross(). do cross(vec), reduces template bloat this way.
// template<> cross won't need to be instantiated for all vec types.
