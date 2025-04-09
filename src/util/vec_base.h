#pragma once

#include "types.h"
#include <utility>

EMB_NAMESPACE_START

// usual containers

// vec2/3/4 etc

template<typename T = embF32>
class embVec2 {
  public:
    using class_type = embVec2<T>;
    using value_type = T;
    //using size_type = decltype(s_VecSize);
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    static constexpr embU32 s_VecSize = 2;
    using size_type = embU32;

  public:
    value_type x = 0.f;
    value_type y = 0.f;
    reference r = x;
    reference g = y;

  public:
    // Constructors & frens
    constexpr embVec2<T>(const embVec2<T>& rhs) noexcept : x{rhs.x}, y{rhs.y} {};
    constexpr embVec2<T>(const embVec2<T>&& rhs) noexcept
        : x{std::move(rhs.x)}, y{std::move(rhs.y)} {};
    constexpr class_type& operator=(const class_type& rhs) noexcept
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }
    constexpr class_type& operator=(const class_type&& rhs) noexcept
    {
        x = std::move(rhs.x);
        y = std::move(rhs.y);
        return *this;
    }
    constexpr embVec2<T>(value_type x_ = 0, value_type y_ = 0) noexcept : x{x_}, y{y_} {};

    constexpr class_type& Set(value_type x_, value_type y_)
    {
        x = x_;
        y = y_;
        return *this;
    }

    constexpr bool operator==(const class_type& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y;
    };

    reference operator[](size_type i) { return *((value_type*)this + i); }
    const_reference operator[](size_type i) const { return *((value_type*)this + i); }

    constexpr class_type operator-() const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = -(*this)[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator+(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] + rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator+(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] + rhs;
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator-(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] - rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator-(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] - rhs;
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator*(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] * rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator*(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] * rhs;
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator/(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] / rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator/(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] / rhs;
        }
        return ret;
    }

    class_type& operator+=(const class_type& rhs) noexcept
    {
        *this = *this + rhs;
        return *this;
    }

    class_type& operator+=(value_type rhs) noexcept
    {
        *this = *this + rhs;
        return *this;
    }

    class_type& operator-=(const class_type& rhs) noexcept
    {
        *this = *this - rhs;
        return *this;
    }

    class_type& operator-=(value_type rhs) noexcept
    {
        *this = *this - rhs;
        return *this;
    }

    class_type& operator*=(const class_type& rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    class_type& operator*=(value_type rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    class_type& operator/=(const class_type& rhs) noexcept
    {
        *this = *this / rhs;
        return *this;
    }

    class_type& operator/=(value_type rhs) noexcept
    {
        *this = *this / rhs;
        return *this;
    }
};

template<typename T = embF32>
class embVec3 {
  public:
    using class_type = embVec3<T>;
    using value_type = T;
    //using size_type = decltype(s_VecSize);
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    static constexpr embU32 s_VecSize = 3;
    using size_type = embU32;

  public:
    value_type x = 0.f;
    value_type y = 0.f;
    value_type z = 0.f;
    reference r = x;
    reference g = y;
    reference b = z;

  public:
    // Constructors & frens
    constexpr embVec3<T>(const embVec3<T>& rhs) noexcept : x{rhs.x}, y{rhs.y}, z{rhs.z} {};
    constexpr embVec3<T>(const embVec3<T>&& rhs) noexcept
        : x{std::move(rhs.x)}, y{std::move(rhs.y)}, z{std::move(rhs.z)} {};
    constexpr class_type& operator=(const class_type& rhs) noexcept
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }
    constexpr class_type& operator=(const class_type&& rhs) noexcept
    {
        x = std::move(rhs.x);
        y = std::move(rhs.y);
        z = std::move(rhs.z);
        return *this;
    }
    constexpr embVec3<T>(value_type x_ = 0, value_type y_ = 0, value_type z_ = 0) noexcept
        : x{x_}, y{y_}, z{z_} {};

    constexpr class_type& Set(value_type x_, value_type y_, value_type z_)
    {
        x = x_;
        y = y_;
        z = z_;
        return *this;
    }

    constexpr bool operator==(const class_type& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    };

    reference operator[](size_type i) { return *((value_type*)this + i); }
    const_reference operator[](size_type i) const { return *((value_type*)this + i); }

    constexpr class_type operator-() const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = -(*this)[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator+(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] + rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator+(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] + rhs;
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator-(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] - rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator-(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] - rhs;
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator*(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] * rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator*(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] * rhs;
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator/(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] / rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator/(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] / rhs;
        }
        return ret;
    }

    class_type& operator+=(const class_type& rhs) noexcept
    {
        *this = *this + rhs;
        return *this;
    }

    class_type& operator+=(value_type rhs) noexcept
    {
        *this = *this + rhs;
        return *this;
    }

    class_type& operator-=(const class_type& rhs) noexcept
    {
        *this = *this - rhs;
        return *this;
    }

    class_type& operator-=(value_type rhs) noexcept
    {
        *this = *this - rhs;
        return *this;
    }

    class_type& operator*=(const class_type& rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    class_type& operator*=(value_type rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    class_type& operator/=(const class_type& rhs) noexcept
    {
        *this = *this / rhs;
        return *this;
    }

    class_type& operator/=(value_type rhs) noexcept
    {
        *this = *this / rhs;
        return *this;
    }
};

template<typename T = embF32>
class embVec4 {
  public:
    using class_type = embVec4<T>;
    using value_type = T;
    //using size_type = decltype(s_VecSize);
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    static constexpr embU32 s_VecSize = 4;
    using size_type = embU32;

  public:
    value_type x = 0.f;
    value_type y = 0.f;
    value_type z = 0.f;
    value_type w = 0.f;
    reference r = x;
    reference g = y;
    reference b = z;
    reference a = w;

  public:
    // Constructors & frens
    constexpr embVec4<T>(const embVec4<T>& rhs) noexcept
        : x{rhs.x}, y{rhs.y}, z{rhs.z}, w{rhs.w} {};
    constexpr embVec4<T>(const embVec4<T>&& rhs) noexcept
        : x{std::move(rhs.x)}, y{std::move(rhs.y)}, z{std::move(rhs.z)}, w{std::move(rhs.w)} {};
    constexpr class_type& operator=(const class_type& rhs) noexcept
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
        return *this;
    }
    constexpr class_type& operator=(const class_type&& rhs) noexcept
    {
        x = std::move(rhs.x);
        y = std::move(rhs.y);
        z = std::move(rhs.z);
        w = std::move(rhs.w);
        return *this;
    }
    constexpr embVec4<T>(value_type x_ = 0, value_type y_ = 0, value_type z_ = 0,
                         value_type w_ = 0) noexcept
        : x{x_}, y{y_}, z{z_}, w{w_} {};

    constexpr class_type& Set(value_type x_, value_type y_, value_type z_, value_type w_)
    {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
        return *this;
    }

    constexpr bool operator==(const class_type& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
    };

    reference operator[](size_type i) { return *((value_type*)this + i); }
    const_reference operator[](size_type i) const { return *((value_type*)this + i); }

    constexpr class_type operator-() const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = -(*this)[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator+(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] + rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator+(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] + rhs;
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator-(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] - rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator-(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] - rhs;
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator*(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] * rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator*(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] * rhs;
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator/(const class_type& rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] / rhs[i];
        }
        return ret;
    }

    [[nodiscard]]
    class_type operator/(value_type rhs) const noexcept
    {
        class_type ret;
        for (size_type i = 0; i < s_VecSize; i++)
        {
            ret[i] = (*this)[i] / rhs;
        }
        return ret;
    }

    class_type& operator+=(const class_type& rhs) noexcept
    {
        *this = *this + rhs;
        return *this;
    }

    class_type& operator+=(value_type rhs) noexcept
    {
        *this = *this + rhs;
        return *this;
    }

    class_type& operator-=(const class_type& rhs) noexcept
    {
        *this = *this - rhs;
        return *this;
    }

    class_type& operator-=(value_type rhs) noexcept
    {
        *this = *this - rhs;
        return *this;
    }

    class_type& operator*=(const class_type& rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    class_type& operator*=(value_type rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    class_type& operator/=(const class_type& rhs) noexcept
    {
        *this = *this / rhs;
        return *this;
    }

    class_type& operator/=(value_type rhs) noexcept
    {
        *this = *this / rhs;
        return *this;
    }
};

using embVec2S = embVec2<embS32>;
using embVec2U = embVec2<embU32>;
using embVec3S = embVec3<embS32>;
using embVec3U = embVec3<embU32>;
using embVec4S = embVec4<embS32>;
using embVec4U = embVec4<embU32>;

// For pre-compilation of common classes

#define EXTERN_MACRO(valueType) \
    extern template class embVec2<valueType>; \
    extern template class embVec3<valueType>; \
    extern template class embVec4<valueType>;

EXTERN_MACRO(embF32)
// EXTERN_MACRO(embF64)
EXTERN_MACRO(embU8)
// EXTERN_MACRO(embS32)
// EXTERN_MACRO(embU32)

#undef EXTERN_MACRO

EMB_NAMESPACE_END


// TODO
// Global overload for +-*/ for left/right alternating sides. Move out of member operator.
// Some ops dont make sense like vec / vec wtf lol. Or num / vec. special case is vec * vec, component multiply.
// Vector specific ops, like cross/angle.
