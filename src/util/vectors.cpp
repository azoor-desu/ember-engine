#include "Utils/Vectors.h"
#include "Utils/Math.h" // FastInvSqrt
#include "Utils/Utils.h" // AlmostEqual
#include <cmath> // cos, sin
#include <iomanip> // stream output overload


namespace LE
{
/****************************************************************************/
/* ============================= Vec2 Stuff ============================= */
/****************************************************************************/

// ============= Static Properties ===============
const Vec2 Vec2::zero = Vec2(0.f, 0.f); // Represents (0, 0)
const Vec2 Vec2::one = Vec2(1.f, 1.f); // Represents (1, 1)
const Vec2 Vec2::left = Vec2(-1.f, 0.f); // Represents (-1, 0)
const Vec2 Vec2::right = Vec2(1.f, 0.f); // Represents (1, 0)
const Vec2 Vec2::up = Vec2(0.f, 1.f); // Represents (0, 1)
const Vec2 Vec2::down = Vec2(0.f, -1.f); // Represents (0, -1)

// =============== Vec2 Constructors ===============

// Default constructor
Vec2::Vec2(float x, float y) : x{x}, y{y} {}

// =============== Assignment operators ===============

// Adds vector to vector then assigns to this vector
Vec2& Vec2::operator+=(const Vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}
// Minus vector from vector then assigns to this vector
Vec2& Vec2::operator-=(const Vec2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}
// Multiplies vector and a scalar value, then assigns to this vector
Vec2& Vec2::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;
    return *this;
}
// Divides x and y values by a scalar value respectively, then assigns to this vector.
Vec2& Vec2::operator/=(float rhs)
{
    x /= rhs;
    y /= rhs;
    return *this;
}

// =============== Comparison Operators ===============

// Checks if all values in both vectors are almost the same.
bool Vec2::operator==(const Vec2& rhs) const
{
    if (x == rhs.x && y == rhs.y)
        return true;
    else
        return false;
}

// Checks if all values in both vectors are different
bool Vec2::operator!=(const Vec2& rhs) const { return !(*this == rhs); }

// ============= Unary operators ===============

// Returns the negation of this vector.
Vec2 Vec2::operator-() const { return Vec2(-x, -y); }

// ============= Accessor operators ===============

// Allows access to x, y values using [] operator. Returns mutable values.
float& Vec2::operator[](size_t index)
{
    if (index == 0)
        return x;
    else
        return y;
    // TODO Error here if out of range
}
// Allows access to x, y values using [] operator. Returns immutable values.
const float Vec2::operator[](size_t index) const
{
    if (index == 0)
        return x;
    else
        return y;
    // TODO Error here if out of range
}

// ============= Non-Static Member functions ===============

// Sets the x and y components of this vector.
// Returns a reference of itself.
Vec2& Vec2::Set(float _x, float _y)
{
    x = _x;
    y = _y;
    return *this;
}

// Returns a copy of this vector with a length of 1.
Vec2 Vec2::Normalized() const { return *this / Length(); }

// Returns a copy of this vector with a length of 1.
// Avoids use of sqrt but slightly more inaccurate. Accurate up to about 4dp.
Vec2 Vec2::FastNormalized() const { return (*this) * FastInvSqrt(SquareLength()); }

// Returns the length of this vector
float Vec2::Length() const { return sqrtf(SquareLength()); }

// Returns the square of this vector's length. Avoids the square root
float Vec2::SquareLength() const { return x * x + y * y; }

// Converts this vector into Vec3. Z value defaults to 0.
Vec3 Vec2::ToVec3(float z) const { return Vec3{x, y, z}; }

// Converts this vector into Vec4.
Vec4 Vec2::ToVec4(float z, float w) const { return Vec4{x, y, z, w}; }

// ============= Static Member functions ===============

// Creates a new normalized vector given a rotation in radians in ccw direction.
Vec2 Vec2::CreateVectorDir(float radians) { return Vec2(cosf(radians), sinf(radians)); }

// Returns the distance between 2 points
float Vec2::Distance(const Vec2& pt0, const Vec2& pt1)
{
    return sqrtf(Vec2::SquareDistance(pt0, pt1));
}

// Returns the squared distance between 2 points. Avoids the square root
float Vec2::SquareDistance(const Vec2& pt0, const Vec2& pt1) { return (pt0 - pt1).SquareLength(); }

// Returns a vector rotated by a certain amount in radians in the ccw direction.
Vec2 Vec2::RotateVector(const Vec2& vec, float radians)
{
    float cosTheta = cosf(radians);
    float sinTheta = sinf(radians);
    return Vec2(vec.x * cosTheta - vec.y * sinTheta, vec.x * sinTheta + vec.y * cosTheta);
}

// Returns the smaller (acute) uninged angle between 2 vectors.
// Ranges from 0 to 180 degrees (in radian units).
// Does not matter which angle comes first, the absolute value between
// the angles will be returned.
float Vec2::AngleBtwnUnsigned(const Vec2& from, const Vec2& to)
{
    return acosf(Vec2::Dot(from, to) / sqrt(from.SquareLength() * to.SquareLength()));
}

// Returns the smaller (acute) singed angle between 2 vectors.
// Ranges from -180 to 180 degrees (in radian units).
// Value will be positive if "to" vector is ccw-wise of "from" vector.
// Else the value will be -ve.
float Vec2::AngleBtwnSigned(const Vec2& from, const Vec2& to)
{
    // TO dot with FROMLEFTNORMAL. +ve, LEFT side of TO, which is ccw.
    Vec2 fromLeftNormal(-from.y, from.x);
    if (Vec2::Dot(fromLeftNormal, to) < 0)
        return -AngleBtwnUnsigned(from, to);
    else
        return AngleBtwnUnsigned(from, to);
}

// Returns the smaller (acute) uninged angle between 2 vectors.
// Ranges from 0 to 180 degrees (in radian units).
// Does not matter which angle comes first, the absolute value between
// the angles will be returned. Faster but less accurate version.
float Vec2::AngleBtwnUnsignedFast(const Vec2& from, const Vec2& to)
{
    return acosf(Vec2::Dot(from, to) * FastInvSqrt(from.SquareLength() * to.SquareLength()));
}

// Returns the smaller (acute) singed angle between 2 vectors.
// Ranges from -180 to 180 degrees (in radian units).
// Value will be positive if "to" vector is ccw-wise of "from" vector.
// Else the value will be -ve. Faster but less accurate version.
float Vec2::AngleBtwnSignedFast(const Vec2& from, const Vec2& to)
{
    // TO dot with FROMLEFTNORMAL. +ve, LEFT side of TO, which is ccw.
    Vec2 fromLeftNormal(-from.y, from.x);
    if (Vec2::Dot(fromLeftNormal, to) < 0)
        return -AngleBtwnUnsignedFast(from, to);
    else
        return AngleBtwnUnsignedFast(from, to);
}

// Returns the dot product of vec0 and vec1
float Vec2::Dot(const Vec2& vec0, const Vec2& vec1) { return vec0.x * vec1.x + vec0.y * vec1.y; }

// Multiplies two vectors component-wise.
Vec2 Vec2::ComponentMul(const Vec2& vec0, const Vec2& vec1)
{
    return Vec2{vec0.x * vec1.x, vec0.y * vec1.y};
}

// Returns the magnitude (length) of the cross product vector of vec0 X vec1
float Vec2::CrossMagnitude(const Vec2& vec0, const Vec2& vec1)
{
    //u1v2 - u2v1
    return vec0.x * vec1.y - vec0.y * vec1.x;
}

// Returns the magnitude (length) of the 2 vectors of a triangle using 3 points.
// If +ve value, points have counter-clockwise winding.
float Vec2::CrossMagnitude(const Vec2& pt0, const Vec2& pt1, const Vec2& pt2)
{
    return CrossMagnitude(pt1 - pt0, pt2 - pt0);
}

// Returns the projection of vec0 onto vec1.
// vec1 will be the "floor" of the projection.
Vec2 Vec2::Project(const Vec2& vec0, const Vec2& vec1)
{
    // (U.V / V.V) * V if V is not normalized. Else, (U.VHat) * VHat is faster.
    // Check if vec1 has length of 1.
    float vec1Length = vec1.SquareLength();
    if (AlmostEqualDp(vec1Length, 1.f, 4))
    {
        return Vec2::Dot(vec0, vec1) * vec1;
    }
    else
    {
        return (Vec2::Dot(vec0, vec1) / vec1Length) * vec1;
    }
}

// =================================== Vec2 Global Overload Operators ===================================

// ============= Binary operators ===============

// Adds vector to vector then returns the resultant vector.
Vec2 operator+(const Vec2& lhs, const Vec2& rhs) { return Vec2(lhs.x + rhs.x, lhs.y + rhs.y); }
// Subtracts vector from vector then returns the resultant vector.
Vec2 operator-(const Vec2& lhs, const Vec2& rhs) { return Vec2(lhs.x - rhs.x, lhs.y - rhs.y); }
//Multiplies vector with scalar then returns the resultant vector.
Vec2 operator*(const Vec2& lhs, float rhs) { return Vec2(lhs.x * rhs, lhs.y * rhs); }
// Multiplies scalar value with a vector then returns the resultant vector.
Vec2 operator*(float lhs, const Vec2& rhs) { return Vec2(lhs * rhs.x, lhs * rhs.y); }
// Divides vector with a scalar value then returns the resultant vector.
Vec2 operator/(const Vec2& lhs, float rhs) { return Vec2(lhs.x / rhs, lhs.y / rhs); }

// ============= Stream operators =============

// Writes a formatted vector2D outout to the out stream provided.
std::ostream& operator<<(std::ostream& os, const Vec2& vec)
{
    os << std::fixed << std::setprecision(4);
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}
// ********************************* END Vec2 *********************************

/**************************************************************************/
/* ============================= Vec3 Stuff ============================= */
/**************************************************************************/

// ============= Static Properties ===============
const Vec3 Vec3::zero = Vec3(0.f, 0.f, 0.f); // Represents (0, 0, 0)
const Vec3 Vec3::one = Vec3(1.f, 1.f, 1.f); // Represents (1, 1, 1)
const Vec3 Vec3::left = Vec3(-1.f, 0.f, 0.f); // Represents (-1, 0, 0)
const Vec3 Vec3::right = Vec3(1.f, 0.f, 0.f); // Represents (1, 0, 0)
const Vec3 Vec3::up = Vec3(0.f, 1.f, 0.f); // Represents (0, 1, 0)
const Vec3 Vec3::down = Vec3(0.f, -1.f, 0.f); // Represents (0, -1, 0)
const Vec3 Vec3::forward = Vec3(0.f, 0.f, 1.f); // Represents (0, 0, 1)
const Vec3 Vec3::back = Vec3(0.f, 0.f, -1.f); // Represents (0, 0, -1)

// =============== Vec3 Constructors ===============

// Default constructor
Vec3::Vec3(float x, float y, float z) : x{x}, y{y}, z{z} {};

// Pseudo-conversion constructor
Vec3::Vec3(Vec2 vec, float z) : x{vec.x}, y{vec.y}, z{z} {};

// =============== Assignment operators ===============

// Adds vector to vector then assigns to this vector
Vec3& Vec3::operator+=(const Vec3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}
// Minus vector from vector then assigns to this vector
Vec3& Vec3::operator-=(const Vec3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}
// Multiplies vector and a scalar value, then assigns to this vector
Vec3& Vec3::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}
// Divides x and y values by a scalar value respectively, then assigns to this vector.
Vec3& Vec3::operator/=(float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

// =============== Comparison Operators ===============

// Checks if all values in both vectors are almost the same.
bool Vec3::operator==(const Vec3& rhs) const
{
    if (x == rhs.x && y == rhs.y && z == rhs.z)
        return true;
    else
        return false;
}
// Checks if all values in both vectors are different
bool Vec3::operator!=(const Vec3& rhs) const { return !(*this == rhs); }

// ============= Unary operators ===============

// Returns the negation of this vector.
Vec3 Vec3::operator-() const { return Vec3(-x, -y, -z); }

// ============= Accessor operators ===============

// Allows access to x, y and z values using [] operator. Returns mutable values.
float& Vec3::operator[](size_t index)
{
    if (index == 0)
        return x;
    if (index == 1)
        return y;
    if (index == 2)
        return z;
    // TODO Error here
    return x;
}
// Allows access to x, y and z values using [] operator. Returns immutable values.
const float Vec3::operator[](size_t index) const
{
    if (index == 0)
        return x;
    if (index == 1)
        return y;
    if (index == 2)
        return z;
    // TODO Error here
    return x;
}

// ============= Non-Static Member functions ===============

// Sets the x y z components of this vector.
// Returns a reference of itself.
Vec3& Vec3::Set(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
    return *this;
}

// Returns a copy of this vector with a length of 1.
Vec3 Vec3::Normalized() const { return (*this) / Length(); }

// Returns a copy of this vector with a length of 1.
// Avoids use of sqrt but slightly more inaccurate. Accurate up to about 4dp.
Vec3 Vec3::FastNormalized() const { return (*this) * FastInvSqrt(SquareLength()); }

// Returns the length of this vector
float Vec3::Length() const { return sqrtf(SquareLength()); }

// Returns the square of this vector's length. Avoids the square root
float Vec3::SquareLength() const { return x * x + y * y + z * z; }

// Converts this vector into Vec2. Z value is discarded.
Vec2 Vec3::ToVec2() const { return Vec2{x, y}; }

// Converts this vector into Vec4.
Vec4 Vec3::ToVec4(float w) const { return Vec4{x, y, z, w}; }

// ============= Static Member functions ===============

// Returns the distance between 2 points
float Vec3::Distance(const Vec3& vec0, const Vec3& vec1)
{
    return sqrtf(SquareDistance(vec0, vec1));
}

// Returns the squared distance between 2 points. Avoids square root.
float Vec3::SquareDistance(const Vec3& vec0, const Vec3& vec1)
{
    return (vec0 - vec1).SquareLength();
}

// Returns the vector rotated by a certain amount in radians.
Vec3 Vec3::RotateVector(const Vec3& vec, float radians, const Vec3& axis)
{
    // Using formula v' = (1-cos(angle)).Projn(V) + v.cos(angle) + (n x v).sin(angle)
    // axis is normal. small n.
    Vec3 normal = axis.Normalized();
    float cosTheta = cosf(radians);
    return (1.f - cosTheta) * Vec3::Projection(vec, normal) + vec * cosTheta
           + Vec3::Cross(normal, vec) * sinf(radians);
}

// Returns the smaller (acute) singed angle between 2 vectors.
// Ranges from 0 to 180 degrees (in radian units).
// Does not matter which angle comes first, the absolute value between
// the angles will be returned.
float Vec3::AngleBtwnUnsigned(const Vec3& from, const Vec3& to)
{
    return acosf(Vec3::Dot(from, to) / sqrt(from.SquareLength() * to.SquareLength()));
}

// Returns the smaller (acute) singed angle between 2 vectors.
// Ranges from -180 to 180 degrees (in radian units).
// Value will be positive if "to" vector is ccw-wise of "from" vector.
// Else the value will be -ve.
// Axis is used to determine the signd-ness of the angle between. Rotation ccw about axis vector gives positive value.
// Axis should be a cross product of from and to. If axis is not orthogonal to from and to, result is undefined.
float Vec3::AngleBtwnSigned(const Vec3& from, const Vec3& to, const Vec3& axis)
{
    // See if normal vector of fromxto plane has same dir as axis vector using dot product.
    Vec3 norm = Vec3::Cross(from, to);
    if (Vec3::Dot(norm, axis) < 0)
        return -AngleBtwnUnsigned(from, to);
    else
        return AngleBtwnUnsigned(from, to);
}

// Returns the smaller (acute) uninged angle between 2 vectors.
// Ranges from 0 to 180 degrees (in radian units).
// Does not matter which angle comes first, the absolute value between
// the angles will be returned. Faster but less accurate version.
float Vec3::AngleBtwnUnsignedFast(const Vec3& from, const Vec3& to)
{
    return acosf(Vec3::Dot(from, to) * FastInvSqrt(from.SquareLength() * to.SquareLength()));
}

// Returns the smaller (acute) singed angle between 2 vectors.
// Ranges from -180 to 180 degrees (in radian units).
// Value will be positive if "to" vector is ccw-wise of "from" vector.
// Else the value will be -ve. Faster but less accurate version.
float Vec3::AngleBtwnSignedFast(const Vec3& from, const Vec3& to, const Vec3& axis)
{
    // See if normal vector of fromxto plane has same dir as axis vector using dot product.
    Vec3 norm = Vec3::Cross(from, to);
    if (Vec3::Dot(norm, axis) < 0)
        return -AngleBtwnUnsignedFast(from, to);
    else
        return AngleBtwnUnsignedFast(from, to);
}

// Returns the dot product between vec0 and vec1
float Vec3::Dot(const Vec3& vec0, const Vec3& vec1)
{
    return vec0.x * vec1.x + vec0.y * vec1.y + vec0.z * vec1.z;
}

// Multiplies two vectors component-wise.
Vec3 Vec3::ComponentMul(const Vec3& vec0, const Vec3& vec1)
{
    return Vec3{vec0.x * vec1.x, vec0.y * vec1.y, vec0.z * vec1.z};
}

// Returns the cross product vector of vec0 cross vec1
Vec3 Vec3::Cross(const Vec3& vec0, const Vec3& vec1)
{
    Vec3 ret;
    ret.x = vec0.y * vec1.z - vec0.z * vec1.y;
    ret.y = -(vec0.x * vec1.z - vec0.z * vec1.x);
    ret.z = vec0.x * vec1.y - vec0.y * vec1.x;
    return ret;
}

// Returns the squared magnitude (length) of the cross product vector of vec0 cross vec1
float Vec3::CrossSquareMagnitude(const Vec3& vec0, const Vec3& vec1)
{
    return Vec3::Cross(vec0, vec1).SquareLength();
}

// Returns the magnitude (length) of the cross product vector of vec0 cross vec1
float Vec3::CrossMagnitude(const Vec3& vec0, const Vec3& vec1)
{
    return Vec3::Cross(vec0, vec1).Length();
}

// Returns the projection of vec0 onto vec1.
// vec1 will be the "floor" of the projection, will be normalized if not already normalized.
Vec3 Vec3::Projection(const Vec3& vec0, const Vec3& vec1)
{
    // (U.V / V.V) * V if V is not normalized. Else, (U.VHat) * VHat is faster.
    // Check if vec1 has length of 1.
    float vec1Length = vec1.SquareLength();
    if (AlmostEqualDp(vec1Length, 1.f, 4))
    {
        return Vec3::Dot(vec0, vec1) * vec1;
    }
    else
    {
        return (Vec3::Dot(vec0, vec1) / vec1Length) * vec1;
    }
}

// =================== Vec3 Global Overload Operators =======================

// ============= Binary operators ===============

// Adds vector to vector then returns the resultant vector.
Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
// Subtracts vector from vector then returns the resultant vector.
Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
// Multiplies vector with scalar then returns the resultant vector.
Vec3 operator*(const Vec3& lhs, float rhs) { return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
// Multiplies scalar value with a vector then returns the resultant vector.
Vec3 operator*(float lhs, const Vec3& rhs) { return Vec3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }
// Divides vector with a scalar value then returns the resultant vector.
Vec3 operator/(const Vec3& lhs, float rhs) { return Vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }

// ============= Stream operators =============

// Writes a formatted vector3D outout to the out stream provided.
std::ostream& operator<<(std::ostream& os, const Vec3& vec)
{
    os << std::fixed << std::setprecision(4);
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}
// ********************************* END Vec3 *********************************

/**************************************************************************/
/* ============================= Vec4 Stuff ============================= */
/**************************************************************************/

// ============= Static Properties ===============
const Vec4 Vec4::zero = Vec4(0.f, 0.f, 0.f); // Represents (0, 0, 0, 0)
const Vec4 Vec4::one = Vec4(1.f, 1.f, 1.f); // Represents (1, 1, 1, 1)

// =============== Vec4 Constructors ===============

// Default constructor
Vec4::Vec4(float x, float y, float z, float w) : x{x}, y{y}, z{z}, w{w} {};

// Pseudo-conversion constructors
Vec4::Vec4(Vec2 vec, float z, float w) : x{vec.x}, y{vec.y}, z{z}, w{w} {};
Vec4::Vec4(Vec3 vec, float w) : x{vec.x}, y{vec.y}, z{vec.z}, w{w} {};

// =============== Assignment operators ===============

// Adds vector to vector then assigns to this vector
Vec4& Vec4::operator+=(const Vec4& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
}
// Minus vector from vector then assigns to this vector
Vec4& Vec4::operator-=(const Vec4& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
}
// Multiplies vector and a scalar value, then assigns to this vector
Vec4& Vec4::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    return *this;
}
// Divides x and y values by a scalar value respectively, then assigns to this vector.
Vec4& Vec4::operator/=(float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
    return *this;
}

// =============== Comparison Operators ===============

// Checks if all values in both vectors are almost the same.
bool Vec4::operator==(const Vec4& rhs) const
{
    if (AlmostEqualDp(this->x, rhs.x, 4) && AlmostEqualDp(this->y, rhs.y, 4)
        && AlmostEqualDp(this->z, rhs.z, 4) && AlmostEqualDp(this->w, rhs.w, 4))
        return true;
    else
        return false;
}
// Checks if all values in both vectors are different
bool Vec4::operator!=(const Vec4& rhs) const { return !(*this == rhs); }

// ============= Unary operators ===============

// Returns the negation of this vector.
Vec4 Vec4::operator-() const { return Vec4(-x, -y, -z, -w); }

// ============= Accessor operators ===============

// Allows access to x, y and z values using [] operator. Returns mutable values.
float& Vec4::operator[](size_t index)
{
    if (index == 0)
        return x;
    if (index == 1)
        return y;
    if (index == 2)
        return z;
    if (index == 3)
        return w;
    // TODO Error here
    return x;
}
// Allows access to x, y and z values using [] operator. Returns immutable values.
const float Vec4::operator[](size_t index) const
{
    if (index == 0)
        return x;
    if (index == 1)
        return y;
    if (index == 2)
        return z;
    if (index == 3)
        return w;
    // TODO Error here
    return x;
}

// ============= Non-Static Member functions ===============

// Sets the x y z components of this vector.
// Returns a reference of itself.
Vec4& Vec4::Set(float _x, float _y, float _z, float _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
    return *this;
}

// Returns a copy of this vector with a length of 1.
Vec4 Vec4::Normalized() const { return (*this) / Length(); }

// Returns a copy of this vector with a length of 1.
// Avoids use of sqrt but slightly more inaccurate. Accurate up to about 4dp.
Vec4 Vec4::FastNormalized() const { return (*this) * FastInvSqrt(SquareLength()); }

// Returns the length of this vector
float Vec4::Length() const { return sqrtf(SquareLength()); }

// Returns the square of this vector's length. Avoids the square root
float Vec4::SquareLength() const { return x * x + y * y + z * z + w * w; }

// Converts this vector into Vec2. Z and W values are discarded.
Vec2 Vec4::ToVec2() const { return Vec2{x, y}; }

// Converts this vector into Vec3. W value is discarded.
Vec3 Vec4::ToVec3() const { return Vec3{x, y, z}; }

// ============= Static Member functions ===============

// Returns the distance between 2 points
float Vec4::Distance(const Vec4& vec0, const Vec4& vec1)
{
    return sqrtf(SquareDistance(vec0, vec1));
}

// Returns the squared distance between 2 points. Avoids square root.
float Vec4::SquareDistance(const Vec4& vec0, const Vec4& vec1)
{
    return (vec0 - vec1).SquareLength();
}

// Returns the dot product between vec0 and vec1
float Vec4::Dot(const Vec4& vec0, const Vec4& vec1)
{
    return vec0.x * vec1.x + vec0.y * vec1.y + vec0.z * vec1.z + vec0.w * vec1.w;
}

// Multiplies two vectors component-wise.
Vec4 Vec4::ComponentMul(const Vec4& vec0, const Vec4& vec1)
{
    return Vec4{vec0.x * vec1.x, vec0.y * vec1.y, vec0.z * vec1.z, vec0.w * vec1.w};
}

// Returns the projection of vec0 onto vec1.
// vec1 will be the "floor" of the projection, will be normalized if not already normalized.
Vec4 Vec4::Projection(const Vec4& vec0, const Vec4& vec1)
{
    // (U.V / V.V) * V if V is not normalized. Else, (U.VHat) * VHat is faster.
    // Check if vec1 has length of 1.
    float vec1Length = vec1.SquareLength();
    if (AlmostEqualDp(vec1Length, 1.f, 4))
    {
        return Vec4::Dot(vec0, vec1) * vec1;
    }
    else
    {
        return (Vec4::Dot(vec0, vec1) / vec1Length) * vec1;
    }
}

// =================== Vec4 Global Overload Operators =======================

// ============= Binary operators ===============

// Adds vector to vector then returns the resultant vector.
Vec4 operator+(const Vec4& lhs, const Vec4& rhs)
{
    return Vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
// Subtracts vector from vector then returns the resultant vector.
Vec4 operator-(const Vec4& lhs, const Vec4& rhs)
{
    return Vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
// Multiplies vector with scalar then returns the resultant vector.
Vec4 operator*(const Vec4& lhs, float rhs)
{
    return Vec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}
// Multiplies scalar value with a vector then returns the resultant vector.
Vec4 operator*(float lhs, const Vec4& rhs)
{
    return Vec4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
}
// Divides vector with a scalar value then returns the resultant vector.
Vec4 operator/(const Vec4& lhs, float rhs)
{
    return Vec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}

// ============= Stream operators =============

// Writes a formatted vector3D outout to the out stream provided.
std::ostream& operator<<(std::ostream& os, const Vec4& vec)
{
    os << std::fixed << std::setprecision(4);
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << "," << vec.w << ")";
    return os;
}
// ********************************* END Vec4 *********************************
} // namespace LE
