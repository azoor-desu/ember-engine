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
    constexpr embVec2<T>() noexcept = default;
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
    constexpr embVec2<T>(value_type x_, value_type y_) noexcept : x{x_}, y{y_} {};

    constexpr class_type operator-() const noexcept { return class_type(-this->x, -this->y); }
    constexpr bool operator==(const class_type& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y;
    };

    [[nodiscard]]
    class_type operator+(const class_type& rhs) const noexcept
    {
        return class_type(x + rhs.x, y + rhs.y);
    }

    [[nodiscard]]
    class_type operator-(const class_type& rhs) const noexcept
    {
        return class_type(x - rhs.x, y - rhs.y);
    }

    [[nodiscard]]
    class_type operator*(const class_type& rhs) const noexcept
    {
        return class_type(x * rhs.x, y * rhs.y);
    }

    [[nodiscard]]
    class_type operator/(const class_type& rhs) const noexcept
    {
        return class_type(x / rhs.x, y / rhs.y);
    }

    class_type& operator+=(const class_type& rhs) noexcept
    {
        *this = *this + rhs;
        return *this;
    }

    class_type& operator-=(const class_type& rhs) noexcept
    {
        *this = *this - rhs;
        return *this;
    }

    class_type& operator*=(const class_type& rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    class_type& operator/=(const class_type& rhs) noexcept
    {
        *this = *this / rhs;
        return *this;
    }

    reference operator[](size_type i) { return *((value_type*)this + i); }
    const_reference operator[](size_type i) const { return *((value_type*)this + i); }
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

    static constexpr embU32 s_VecSize = 2;
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
    constexpr embVec3<T>() noexcept = default;
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
    constexpr embVec3<T>(value_type x_, value_type y_, value_type z_) noexcept
        : x{x_}, y{y_}, z{z_} {};

    constexpr class_type operator-() const noexcept
    {
        return class_type(-this->x, -this->y, -this->z, );
    }
    constexpr bool operator==(const class_type& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    };

    [[nodiscard]]
    class_type operator+(const class_type& rhs) const noexcept
    {
        return class_type(x + rhs.x, y + rhs.y, z + rhs.z, );
    }

    [[nodiscard]]
    class_type operator-(const class_type& rhs) const noexcept
    {
        return class_type(x - rhs.x, y - rhs.y, z - rhs.z, );
    }

    [[nodiscard]]
    class_type operator*(const class_type& rhs) const noexcept
    {
        return class_type(x * rhs.x, y * rhs.y, z * rhs.z, );
    }

    [[nodiscard]]
    class_type operator/(const class_type& rhs) const noexcept
    {
        return class_type(x / rhs.x, y / rhs.y, z / rhs.z, );
    }

    class_type& operator+=(const class_type& rhs) noexcept
    {
        *this = *this + rhs;
        return *this;
    }

    class_type& operator-=(const class_type& rhs) noexcept
    {
        *this = *this - rhs;
        return *this;
    }

    class_type& operator*=(const class_type& rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    class_type& operator/=(const class_type& rhs) noexcept
    {
        *this = *this / rhs;
        return *this;
    }

    reference operator[](size_type i) { return *((value_type*)this + i); }
    const_reference operator[](size_type i) const { return *((value_type*)this + i); }
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

    static constexpr embU32 s_VecSize = 2;
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
    constexpr embVec4<T>() noexcept = default;
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
    constexpr embVec4<T>(value_type x_, value_type y_, value_type z_, value_type w_) noexcept
        : x{x_}, y{y_}, z{z_}, w{w_} {};

    constexpr class_type operator-() const noexcept
    {
        return class_type(-this->x, -this->y, -this->z, -this->w);
    }
    constexpr bool operator==(const class_type& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
    };

    [[nodiscard]]
    class_type operator+(const class_type& rhs) const noexcept
    {
        return class_type(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    [[nodiscard]]
    class_type operator-(const class_type& rhs) const noexcept
    {
        return class_type(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    [[nodiscard]]
    class_type operator*(const class_type& rhs) const noexcept
    {
        return class_type(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }

    [[nodiscard]]
    class_type operator/(const class_type& rhs) const noexcept
    {
        return class_type(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
    }

    class_type& operator+=(const class_type& rhs) noexcept
    {
        *this = *this + rhs;
        return *this;
    }

    class_type& operator-=(const class_type& rhs) noexcept
    {
        *this = *this - rhs;
        return *this;
    }

    class_type& operator*=(const class_type& rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    class_type& operator/=(const class_type& rhs) noexcept
    {
        *this = *this / rhs;
        return *this;
    }

    reference operator[](size_type i) { return *((value_type*)this + i); }
    const_reference operator[](size_type i) const { return *((value_type*)this + i); }
};

using embVec2S = embVec2<embS32>;
using embVec2U = embVec2<embU32>;
using embVec3S = embVec3<embS32>;
using embVec3U = embVec3<embU32>;
using embVec4S = embVec4<embS32>;
using embVec4U = embVec4<embU32>;

// For pre-compilation of common classes in lib
extern template class embVec2<embF32>;
extern template class embVec2<embS32>;
extern template class embVec2<embU32>;

extern template class embVec3<embF32>;
extern template class embVec3<embS32>;
extern template class embVec3<embU32>;

extern template class embVec4<embF32>;
extern template class embVec4<embS32>;
extern template class embVec4<embU32>;

EMB_NAMESPACE_END
