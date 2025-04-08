// TODO: Interop with GLM.

#include "Utils/Matrix.h"
#include <cmath> // for cos, sin
#include <iomanip> // For stream out overload

namespace LE
{
/*******************************************************************************/
/* ============================= Matrix3x3 Stuff ============================= */
/*******************************************************************************/

// ============= Static Properties ===============
const Mat3 Mat3::zero{};
const Mat3 Mat3::identity = Mat3(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

// =============== Constructors ===============

// Constructs matrix given 9 float values
Mat3::Mat3(const float* pArr)
{
    for (int i = 0; i < 9; ++i)
        _vals[i] = pArr[i];
}

// Constructs matrix given 9 float values
Mat3::Mat3(float _00, float _01, float _02, float _10, float _11, float _12, float _20, float _21,
           float _22)
{
    _vals[0] = _00;
    _vals[1] = _01;
    _vals[2] = _02;
    _vals[3] = _10;
    _vals[4] = _11;
    _vals[5] = _12;
    _vals[6] = _20;
    _vals[7] = _21;
    _vals[8] = _22;
}

// Constructs matrix using 3 vectors, one for each column.
Mat3::Mat3(const Vec3& col1, const Vec3& col2, const Vec3& col3)
{
    _vals[0] = col1.x;
    _vals[1] = col2.x;
    _vals[2] = col3.x;
    _vals[3] = col1.y;
    _vals[4] = col2.y;
    _vals[5] = col3.y;
    _vals[6] = col1.z;
    _vals[7] = col2.z;
    _vals[8] = col3.z;
}

// =============== Assignment operators ===============

// Assigns the _valuesvals of the matrix rhs to the currently referenced matrix.
Mat3& Mat3::operator=(const Mat3& rhs)
{
    for (int i = 0; i < 9; ++i)
        _vals[i] = rhs._vals[i];
    return *this;
}

// Performs matrix * matrix multiplication and assigns the result to this matrix
Mat3& Mat3::operator*=(const Mat3& rhs)
{
    Mat3 ret;
    // looping through matrix 1 mtx
    for (int m1_row = 0; m1_row < 3; ++m1_row)
        // for each matrix 1 row, loop through matrix 2 columns
        for (int m2_col = 0; m2_col < 3; ++m2_col)
            // loop through matrix 1 columns to calculate the dot product
            for (int m1_col = 0; m1_col < 3; ++m1_col)
                ret(m1_row, m2_col) += (*this)(m1_row, m1_col) * rhs(m1_col, m2_col);

    *this = ret;
    return *this;
}

// Multiplies a float to every value in a matrix and assigns the result to this matrix
Mat3& Mat3::operator*=(float rhs)
{
    Mat3 ret;
    for (size_t i = 0; i < 9; ++i)
        ret._vals[i] = rhs * _vals[i];
    *this = ret;
    return *this;
}

// =============== Comparison Operators ===============

// Checks if all values in both matrices are exactly the same.
bool Mat3::operator==(const Mat3& rhs) const
{
    for (int i = 0; i < 9; ++i)
        if (_vals[i] != rhs._vals[i])
            return false;
    return true;
}

// Checks if all values in both matrices are different
bool Mat3::operator!=(const Mat3& rhs) const { return !((*this) == rhs); }

// ============= Accessor operators ===============

// Mutable reference to a row specified by row index.
std::array<float, 3>& Mat3::operator[](size_t row) { return _rows[row]; }

// Immutable reference to a row specified by row index.
const std::array<float, 3>& Mat3::operator[](size_t row) const { return _rows[row]; }

// Mutable reference to the value specified by row and column.
float& Mat3::operator()(size_t row, size_t column) { return _vals[row * 3 + column]; }

// Immutable reference to the value specified by row and column.
float Mat3::operator()(size_t row, size_t column) const { return _vals[row * 3 + column]; }

// ============= Non-static Member functions ===============

// Returns the transpose of this matrix.
Mat3 Mat3::Transpose()
{
    return Mat3(_vals[0], _vals[3], _vals[6], _vals[1], _vals[4], _vals[7], _vals[2], _vals[5],
                _vals[8]);
}

// Returns the inverse of this matrix.
// On inversion fail, returns identity matrix.
Mat3 Mat3::Inverse()
{

    float detInv = Determinant();
    //If determinant is 0, stop code.
    if (!detInv)
    {
        // TODO error
        return Mat3::identity;
    }
    detInv = 1.f / detInv;

    // Check if last row is 0, 0, 1. If it is,
    // do the faster matrix inverse method.
    if (_vals[6] == 0.f && _vals[7] == 0.f && _vals[8] == 1.f)
    {

        // Split into 2 matrices[T,S], invert both, then concat them again[S,T].
        // Calculate inverse of 2x2 matrix [S] and apply to 3x3 return matrix.
        Mat3 ret{_vals[4] * detInv,
                 -_vals[1] * detInv,
                 0.f,
                 -_vals[3] * detInv,
                 _vals[0] * detInv,
                 0.f,
                 0.f,
                 0.f,
                 1.f};

        // Do [S] inversed times [T] inversed. Just update the last column.
        ret._vals[2] = ret._vals[0] * -_vals[2] + ret._vals[1] * -_vals[5];
        ret._vals[5] = ret._vals[3] * -_vals[2] + ret._vals[4] * -_vals[5];
        return ret;
    }

    // Normal 3x3 matrix inverse

    // 1. Get determinant (done)
    // 2. Find Cofactor
    // 3. Transpose and apply 1/det to every value.

    // Find cofactor matrix. Google for formula.
    Mat3 ret = Mat3((_vals[4] * _vals[8] - _vals[5] * _vals[7]), // m00
                    -(_vals[3] * _vals[8] - _vals[6] * _vals[5]), // m01
                    (_vals[3] * _vals[7] - _vals[6] * _vals[4]), // m02
                    -(_vals[1] * _vals[8] - _vals[7] * _vals[2]), // m10
                    (_vals[0] * _vals[8] - _vals[6] * _vals[2]), // m11
                    -(_vals[0] * _vals[7] - _vals[6] * _vals[1]), // m12
                    (_vals[1] * _vals[5] - _vals[4] * _vals[2]), // m20
                    -(_vals[0] * _vals[5] - _vals[3] * _vals[2]), // m21
                    (_vals[0] * _vals[4] - _vals[3] * _vals[1]) // m22
    );

    //Transpose
    ret = ret.Transpose();

    // Apply 1/det to every value.
    for (int i = 0; i < 9; ++i)
        ret._vals[i] *= detInv;
    return ret;
}

// Returns the determinant of this matrix.
float Mat3::Determinant()
{
    return _vals[0] * (_vals[4] * _vals[8] - _vals[5] * _vals[7])
           - _vals[1] * (_vals[3] * _vals[8] - _vals[6] * _vals[5])
           + _vals[2] * (_vals[3] * _vals[7] - _vals[6] * _vals[4]);
}

// Returns a copy of the specified row of this matrix.
Vec3 Mat3::GetRow(size_t row)
{
    return Vec3(_vals[row * 3 + 0], _vals[row * 3 + 1], _vals[row * 3 + 2]);
}

// Returns a copy of the specified column of this matrix.
Vec3 Mat3::GetCol(size_t col) { return Vec3{_vals[0 + col], _vals[3 + col], _vals[6 + col]}; }

// ============= Static Member functions ===============

// Creates a translate matrix
Mat3 Mat3::CreateTranslateMtx(float x, float y)
{
    return Mat3(1.f, 0.f, x, 0.f, 1.f, y, 0.f, 0.f, 1.f);
}

// Creates a translate matrix
Mat3 Mat3::CreateTranslateMtx(const Vec2& translate)
{
    return CreateTranslateMtx(translate.x, translate.y);
}

// Creates a scale matrix
Mat3 Mat3::CreateScaleMtx(float x, float y)
{
    return Mat3(x, 0.f, 0.f, 0.f, y, 0.f, 0.f, 0.f, 1.f);
}

// Creates a scale matrix
Mat3 Mat3::CreateScaleMtx(const Vec2& scale) { return CreateScaleMtx(scale.x, scale.y); }

// Creates a rotation matrix to rotate any vector
// a certain amt of degrees about the z axis.
// Standard rotation for 2D vectors.
// angle param is in radians.
Mat3 Mat3::CreateRot2DMtx(float radians)
{
    float cosTheta = cosf(radians);
    float sinTheta = sinf(radians);
    return Mat3(cosTheta, -sinTheta, 0.f, sinTheta, cosTheta, 0.f, 0.f, 0.f, 1.f);
}

// Creates a 3D rotation matrix to rotate any vector
// a certain amt of degrees about an arbitrary axis (normal).
// angle param is in radians.
Mat3 Mat3::CreateRot3DMtx(float radians, const Vec3& axis)
{
    // Using formula v' = (1-cos(angle)).Projn(V) + v.cos(angle) + (n x v).sin(angle)
    // axis is normal. small n.
    // Apply to e1 e2 e3 axes to get 3x3 matrix [e1' e2' e3']

    Vec3 normal = axis.Normalized();
    float cosTheta = cosf(radians);
    float sinTheta = sinf(radians);
    float oneMinusCosTheta = 1.f - cosTheta;

    return Mat3(oneMinusCosTheta * Vec3::Projection(Vec3::right, normal) + Vec3::right * cosTheta
                    + Vec3::Cross(normal, Vec3::right) * sinTheta,
                oneMinusCosTheta * Vec3::Projection(Vec3::up, normal) + Vec3::up * cosTheta
                    + Vec3::Cross(normal, Vec3::up) * sinTheta,
                oneMinusCosTheta * Vec3::Projection(Vec3::forward, normal)
                    + Vec3::forward * cosTheta + Vec3::Cross(normal, Vec3::forward) * sinTheta);
}

/// <summary>
/// Creates an affine matrix using scale, rotate and translate in SRT format.
/// </summary>
/// <param name="scale">amount to scale in x,y directions</param>
/// <param name="radians">the angle in radians</param>
/// <param name="pos">amount to translate in x,y directions</param>
/// <param name="axis">the axis (normal)</param>
/// <returns></returns>
Mat3 Mat3::CreateAffineMtx(Vec2 scale, float radians, Vec2 pos)
{
    Mat3 ret = Mat3::identity;
    const float cosrad = cosf(radians);
    const float sinrad = sinf(radians);
    // first 4: S*R
    ret._vals[0] = scale.x * cosrad;
    ret._vals[1] = scale.y * -sinrad;
    ret._vals[3] = scale.x * sinrad;
    ret._vals[4] = scale.y * cosrad;

    // translate
    ret._vals[2] = pos.x;
    ret._vals[5] = pos.y;

    return ret;
}

// =================================== Global Overload Operators ===================================

// ============= Binary operators ===============

// Multiplies a matrix3 * matrix3 and returns the resulting matrix.
Mat3 operator*(const Mat3& lhs, const Mat3& rhs)
{
    Mat3 ret;
    // looping through matrix 1 mtx
    for (int m1_row = 0; m1_row < 3; ++m1_row)
        // for each matrix 1 row, loop through matrix 2 columns
        for (int m2_col = 0; m2_col < 3; ++m2_col)
            // loop through matrix 1 columns to calculate the dot product
            for (int m1_col = 0; m1_col < 3; ++m1_col)
                ret(m1_row, m2_col) += lhs(m1_row, m1_col) * rhs(m1_col, m2_col);
    return ret;
}

// Multiplies a matrix3 * vec3 and returns the resulting vector.
Vec3 operator*(const Mat3& mat, const Vec3& rhs)
{
    Vec3 ret;
    ret.x = mat(0, 0) * rhs.x + mat(0, 1) * rhs.y + mat(0, 2) * rhs.z;
    ret.y = mat(1, 0) * rhs.x + mat(1, 1) * rhs.y + mat(1, 2) * rhs.z;
    ret.z = mat(2, 0) * rhs.x + mat(2, 1) * rhs.y + mat(2, 2) * rhs.z;
    return ret;
}

// Multiplies 3x3 with Vec2 by implicitly converting to Vec3, with last component being 1.
// QOL for manipulating vectors using affine matrices with one missing component.
Vec2 operator*(const Mat3& mat, const Vec2& rhs)
{
    Vec2 ret;
    ret.x = mat(0, 0) * rhs.x + mat(0, 1) * rhs.y + mat(0, 2);
    ret.y = mat(1, 0) * rhs.x + mat(1, 1) * rhs.y + mat(1, 2);
    return ret;
}

// Multiplies a float to every value in a matrix and returns the resulting matrix.
Mat3 operator*(const Mat3& lhs, float rhs)
{
    Mat3 ret;
    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
            ret(i, j) = rhs * lhs(i, j);
    return ret;
}

// Multiplies a float to every value in a matrix and returns the resulting matrix.
Mat3 operator*(float lhs, const Mat3& rhs)
{
    Mat3 ret;
    ret = rhs * lhs;
    return ret;
}

// ============= Stream operators =============

// Prints a nicely formatted 3x3 matrix to output stream
std::ostream& operator<<(std::ostream& os, const Mat3& _rows)
{
    os << std::fixed << std::setprecision(4);
    os << "-------------\n| ";

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            os << _rows(i, j);
            if (j != 2)
                os << " ";
            else
                os << " |\n";
        }
        if (i != 2)
            os << "| ";
    }
    os << "-------------";
    return os;
}
// ********************************* END Mat3 *********************************

/*******************************************************************************/
/* ============================= Matrix4x4 Stuff ============================= */
/*******************************************************************************/

// ============= Static Properties ===============
const Mat4 Mat4::zero{};
const Mat4 Mat4::identity =
    Mat4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f);

// =============== Constructors ===============

// Constructs matrix given 9 float values
Mat4::Mat4(const float* pArr)
{
    for (int i = 0; i < 9; ++i)
        _vals[i] = pArr[i];
}

// Constructs matrix given 9 float values
Mat4::Mat4(float _00, float _01, float _02, float _03, float _10, float _11, float _12, float _13,
           float _20, float _21, float _22, float _23, float _30, float _31, float _32, float _33)
{
    _vals[0] = _00;
    _vals[1] = _01;
    _vals[2] = _02;
    _vals[3] = _03;
    _vals[4] = _10;
    _vals[5] = _11;
    _vals[6] = _12;
    _vals[7] = _13;
    _vals[8] = _20;
    _vals[9] = _21;
    _vals[10] = _22;
    _vals[11] = _23;
    _vals[12] = _30;
    _vals[13] = _31;
    _vals[14] = _32;
    _vals[15] = _33;
}

// Constructs matrix using 3 vectors, one for each column.
Mat4::Mat4(const Vec4& col1, const Vec4& col2, const Vec4& col3, const Vec4& col4)
{
    _vals[0] = col1.x;
    _vals[1] = col2.x;
    _vals[2] = col3.x;
    _vals[3] = col4.x;
    _vals[4] = col1.y;
    _vals[5] = col2.y;
    _vals[6] = col3.y;
    _vals[7] = col4.y;
    _vals[8] = col1.z;
    _vals[9] = col2.z;
    _vals[10] = col3.z;
    _vals[11] = col4.z;
    _vals[12] = col1.w;
    _vals[13] = col2.w;
    _vals[14] = col3.w;
    _vals[15] = col4.w;
}

// =============== Assignment operators ===============

// Assigns the _valuesvals of the matrix rhs to the currently referenced matrix.
Mat4& Mat4::operator=(const Mat4& rhs)
{
    for (int i = 0; i < 16; ++i)
        _vals[i] = rhs._vals[i];
    return *this;
}

// Performs matrix * matrix multiplication and assigns the result to this matrix
Mat4& Mat4::operator*=(const Mat4& rhs)
{
    Mat4 ret;
    // looping through matrix 1 mtx
    for (int m1_row = 0; m1_row < 4; ++m1_row)
        // for each matrix 1 row, loop through matrix 2 columns
        for (int m2_col = 0; m2_col < 4; ++m2_col)
            // loop through matrix 1 columns to calculate the dot product
            for (int m1_col = 0; m1_col < 4; ++m1_col)
                ret(m1_row, m2_col) += (*this)(m1_row, m1_col) * rhs(m1_col, m2_col);

    *this = ret;
    return *this;
}

// Multiplies a float to every value in a matrix and assigns the result to this matrix
Mat4& Mat4::operator*=(float rhs)
{
    Mat4 ret;
    for (size_t i = 0; i < 16; ++i)
        ret._vals[i] = rhs * _vals[i];
    *this = ret;
    return *this;
}

// =============== Comparison Operators ===============

// Checks if all values in both vectors are exactly the same.
bool Mat4::operator==(const Mat4& rhs) const
{
    for (int i = 0; i < 16; ++i)
        if (_vals[i] != rhs._vals[i])
            return false;
    return true;
}

// Checks if all values in both vectors are different
bool Mat4::operator!=(const Mat4& rhs) const { return !((*this) == rhs); }

// ============= Accessor operators ===============

// Mutable reference to a row specified by row index.
std::array<float, 4>& Mat4::operator[](size_t row) { return _rows[row]; }

// Immutable reference to a row specified by row index.
const std::array<float, 4>& Mat4::operator[](size_t row) const { return _rows[row]; }

// Mutable reference to the value specified by row and column.
float& Mat4::operator()(size_t row, size_t column) { return _vals[row * 4 + column]; }

// Immutable reference to the value specified by row and column.
float Mat4::operator()(size_t row, size_t column) const { return _vals[row * 4 + column]; }

// ============= Non-static Member functions ===============

// Returns the transpose of this matrix.
Mat4 Mat4::Transpose()
{
    return Mat4(_vals[0], _vals[4], _vals[8], _vals[12], _vals[1], _vals[5], _vals[9], _vals[13],
                _vals[2], _vals[6], _vals[10], _vals[14], _vals[3], _vals[7], _vals[11], _vals[15]);
}

// Returns the inverse of this matrix.
// On inversion fail, returns identity matrix.
Mat4 Mat4::Inverse()
{
    // https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    float ret[16]{};

    ret[0] = _vals[5] * _vals[10] * _vals[15] - _vals[5] * _vals[11] * _vals[14]
             - _vals[9] * _vals[6] * _vals[15] + _vals[9] * _vals[7] * _vals[14]
             + _vals[13] * _vals[6] * _vals[11] - _vals[13] * _vals[7] * _vals[10];

    ret[4] = -_vals[4] * _vals[10] * _vals[15] + _vals[4] * _vals[11] * _vals[14]
             + _vals[8] * _vals[6] * _vals[15] - _vals[8] * _vals[7] * _vals[14]
             - _vals[12] * _vals[6] * _vals[11] + _vals[12] * _vals[7] * _vals[10];

    ret[8] = _vals[4] * _vals[9] * _vals[15] - _vals[4] * _vals[11] * _vals[13]
             - _vals[8] * _vals[5] * _vals[15] + _vals[8] * _vals[7] * _vals[13]
             + _vals[12] * _vals[5] * _vals[11] - _vals[12] * _vals[7] * _vals[9];

    ret[12] = -_vals[4] * _vals[9] * _vals[14] + _vals[4] * _vals[10] * _vals[13]
              + _vals[8] * _vals[5] * _vals[14] - _vals[8] * _vals[6] * _vals[13]
              - _vals[12] * _vals[5] * _vals[10] + _vals[12] * _vals[6] * _vals[9];

    ret[1] = -_vals[1] * _vals[10] * _vals[15] + _vals[1] * _vals[11] * _vals[14]
             + _vals[9] * _vals[2] * _vals[15] - _vals[9] * _vals[3] * _vals[14]
             - _vals[13] * _vals[2] * _vals[11] + _vals[13] * _vals[3] * _vals[10];

    ret[5] = _vals[0] * _vals[10] * _vals[15] - _vals[0] * _vals[11] * _vals[14]
             - _vals[8] * _vals[2] * _vals[15] + _vals[8] * _vals[3] * _vals[14]
             + _vals[12] * _vals[2] * _vals[11] - _vals[12] * _vals[3] * _vals[10];

    ret[9] = -_vals[0] * _vals[9] * _vals[15] + _vals[0] * _vals[11] * _vals[13]
             + _vals[8] * _vals[1] * _vals[15] - _vals[8] * _vals[3] * _vals[13]
             - _vals[12] * _vals[1] * _vals[11] + _vals[12] * _vals[3] * _vals[9];

    ret[13] = _vals[0] * _vals[9] * _vals[14] - _vals[0] * _vals[10] * _vals[13]
              - _vals[8] * _vals[1] * _vals[14] + _vals[8] * _vals[2] * _vals[13]
              + _vals[12] * _vals[1] * _vals[10] - _vals[12] * _vals[2] * _vals[9];

    ret[2] = _vals[1] * _vals[6] * _vals[15] - _vals[1] * _vals[7] * _vals[14]
             - _vals[5] * _vals[2] * _vals[15] + _vals[5] * _vals[3] * _vals[14]
             + _vals[13] * _vals[2] * _vals[7] - _vals[13] * _vals[3] * _vals[6];

    ret[6] = -_vals[0] * _vals[6] * _vals[15] + _vals[0] * _vals[7] * _vals[14]
             + _vals[4] * _vals[2] * _vals[15] - _vals[4] * _vals[3] * _vals[14]
             - _vals[12] * _vals[2] * _vals[7] + _vals[12] * _vals[3] * _vals[6];

    ret[10] = _vals[0] * _vals[5] * _vals[15] - _vals[0] * _vals[7] * _vals[13]
              - _vals[4] * _vals[1] * _vals[15] + _vals[4] * _vals[3] * _vals[13]
              + _vals[12] * _vals[1] * _vals[7] - _vals[12] * _vals[3] * _vals[5];

    ret[14] = -_vals[0] * _vals[5] * _vals[14] + _vals[0] * _vals[6] * _vals[13]
              + _vals[4] * _vals[1] * _vals[14] - _vals[4] * _vals[2] * _vals[13]
              - _vals[12] * _vals[1] * _vals[6] + _vals[12] * _vals[2] * _vals[5];

    ret[3] = -_vals[1] * _vals[6] * _vals[11] + _vals[1] * _vals[7] * _vals[10]
             + _vals[5] * _vals[2] * _vals[11] - _vals[5] * _vals[3] * _vals[10]
             - _vals[9] * _vals[2] * _vals[7] + _vals[9] * _vals[3] * _vals[6];

    ret[7] = _vals[0] * _vals[6] * _vals[11] - _vals[0] * _vals[7] * _vals[10]
             - _vals[4] * _vals[2] * _vals[11] + _vals[4] * _vals[3] * _vals[10]
             + _vals[8] * _vals[2] * _vals[7] - _vals[8] * _vals[3] * _vals[6];

    ret[11] = -_vals[0] * _vals[5] * _vals[11] + _vals[0] * _vals[7] * _vals[9]
              + _vals[4] * _vals[1] * _vals[11] - _vals[4] * _vals[3] * _vals[9]
              - _vals[8] * _vals[1] * _vals[7] + _vals[8] * _vals[3] * _vals[5];

    ret[15] = _vals[0] * _vals[5] * _vals[10] - _vals[0] * _vals[6] * _vals[9]
              - _vals[4] * _vals[1] * _vals[10] + _vals[4] * _vals[2] * _vals[9]
              + _vals[8] * _vals[1] * _vals[6] - _vals[8] * _vals[2] * _vals[5];

    float det = _vals[0] * ret[0] + _vals[1] * ret[4] + _vals[2] * ret[8] + _vals[3] * ret[12];
    if (det == 0)
        return Mat4::identity; // TODO error

    det = 1.f / det;
    Mat4 retMtx{
        ret[0] * det,  ret[1] * det,  ret[2] * det,  ret[3] * det,  ret[4] * det,  ret[5] * det,
        ret[6] * det,  ret[7] * det,  ret[8] * det,  ret[9] * det,  ret[10] * det, ret[11] * det,
        ret[12] * det, ret[13] * det, ret[14] * det, ret[15] * det,
    };
    return ret;
}

// Returns the determinant of this matrix.
float Mat4::Determinant()
{
    Mat3 laplace{_vals[5],  _vals[6],  _vals[7],  _vals[9], _vals[10],
                 _vals[11], _vals[13], _vals[14], _vals[15]};
    float ret = _vals[0] * laplace.Determinant();

    laplace = {_vals[4],  _vals[6],  _vals[7],  _vals[8], _vals[10],
               _vals[11], _vals[12], _vals[14], _vals[15]};
    ret += _vals[1] * laplace.Determinant();

    laplace = {_vals[4],  _vals[5],  _vals[7],  _vals[8], _vals[9],
               _vals[11], _vals[12], _vals[13], _vals[15]};
    ret += _vals[1] * laplace.Determinant();

    laplace = {_vals[4],  _vals[6],  _vals[6],  _vals[8], _vals[10],
               _vals[10], _vals[12], _vals[14], _vals[14]};
    ret += _vals[1] * laplace.Determinant();

    return ret;
}

// Returns a copy of the specified row of this matrix.
Vec4 Mat4::GetRow(size_t row)
{
    return Vec4(_vals[row * 4], _vals[row * 4 + 1], _vals[row * 4 + 2], _vals[row * 4 + 3]);
}

// Returns a copy of the specified column of this matrix.
Vec4 Mat4::GetCol(size_t col)
{
    return Vec4{_vals[col], _vals[4 + col], _vals[8 + col], _vals[12 + col]};
}

// ============= Static Member functions ===============

// Creates a translate matrix
Mat4 Mat4::CreateTranslateMtx(float x, float y, float z)
{
    return Mat4(1.f, 0.f, 0.f, x, 0.f, 1.f, 0.f, y, 0.f, 0.f, 1.f, z, 0.f, 0.f, 0.f, 1.f);
}

// Creates a translate matrix
Mat4 Mat4::CreateTranslateMtx(const Vec3& translate)
{
    return CreateTranslateMtx(translate.x, translate.y, translate.z);
}

// Creates a scale matrix
Mat4 Mat4::CreateScaleMtx(float x, float y, float z)
{
    return Mat4(x, 0.f, 0.f, 0.f, 0.f, y, 0.f, 0.f, 0.f, 0.f, z, 0.f, 0.f, 0.f, 0.f, 1.f);
}

// Creates a scale matrix
Mat4 Mat4::CreateScaleMtx(const Vec3& scale) { return CreateScaleMtx(scale.x, scale.y, scale.z); }

// Creates a 3D rotation matrix to rotate any vector
// a certain amt of degrees about an arbitrary axis (normal).
// angle param is in radians.
Mat4 Mat4::CreateRot3DMtx(float radians, const Vec3& axis)
{
    // Using formula v' = (1-cos(angle)).Projn(V) + v.cos(angle) + (n x v).sin(angle)
    // axis is normal. small n.
    // Apply to e1 e2 e3 axes to get 3x3 matrix [e1' e2' e3']

    Vec3 normal = axis.Normalized();
    float cosTheta = cosf(radians);
    float sinTheta = sinf(radians);
    float oneMinusCosTheta = 1.f - cosTheta;

    return Mat4(Vec4(oneMinusCosTheta * Vec3::Projection(Vec3::right, normal)
                         + Vec3::right * cosTheta + Vec3::Cross(normal, Vec3::right) * sinTheta,
                     0.f),
                Vec4(oneMinusCosTheta * Vec3::Projection(Vec3::up, normal) + Vec3::up * cosTheta
                         + Vec3::Cross(normal, Vec3::up) * sinTheta,
                     0.f),
                Vec4(oneMinusCosTheta * Vec3::Projection(Vec3::forward, normal)
                         + Vec3::forward * cosTheta + Vec3::Cross(normal, Vec3::forward) * sinTheta,
                     0.f),
                Vec4(0.f, 0.f, 0.f, 1.f));
}

// =================================== Global Overload Operators ===================================

// ============= Binary operators ===============

// Multiplies a matrix4 * matrix4 and returns the resulting matrix.
Mat4 operator*(const Mat4& lhs, const Mat4& rhs)
{
    Mat4 ret;
    // looping through matrix 1 mtx
    for (int m1_row = 0; m1_row < 4; ++m1_row)
        // for each matrix 1 row, loop through matrix 2 columns
        for (int m2_col = 0; m2_col < 4; ++m2_col)
            // loop through matrix 1 columns to calculate the dot product
            for (int m1_col = 0; m1_col < 4; ++m1_col)
                ret(m1_row, m2_col) += lhs(m1_row, m1_col) * rhs(m1_col, m2_col);
    return ret;
}

// Multiplies a matrix4 * vec4 and returns the resulting vector.
Vec4 operator*(const Mat4& mat, const Vec4& rhs)
{
    Vec4 ret;
    ret.x = mat(0, 0) * rhs.x + mat(0, 1) * rhs.y + mat(0, 2) * rhs.z + mat(0, 3) * rhs.w;
    ret.y = mat(1, 0) * rhs.x + mat(1, 1) * rhs.y + mat(1, 2) * rhs.z + mat(1, 3) * rhs.w;
    ret.z = mat(2, 0) * rhs.x + mat(2, 1) * rhs.y + mat(2, 2) * rhs.z + mat(2, 3) * rhs.w;
    ret.w = mat(3, 0) * rhs.x + mat(3, 1) * rhs.y + mat(3, 2) * rhs.z + mat(3, 3) * rhs.w;
    return ret;
}

// Multiplies 4x4 with Vec3 by implicitly converting to Vec4, with last component being 1.
// QOL for manipulating vectors using affine matrices with one missing component.
Vec3 operator*(const Mat4& mat, const Vec3& rhs)
{
    Vec3 ret;
    ret.x = mat(0, 0) * rhs.x + mat(0, 1) * rhs.y + mat(0, 2) * rhs.z + mat(0, 3);
    ret.y = mat(1, 0) * rhs.x + mat(1, 1) * rhs.y + mat(1, 2) * rhs.z + mat(1, 3);
    ret.z = mat(2, 0) * rhs.x + mat(2, 1) * rhs.y + mat(2, 2) * rhs.z + mat(2, 3);
    return ret;
}

// Multiplies a float to every value in a matrix and returns the resulting matrix.
Mat4 operator*(const Mat4& lhs, float rhs)
{
    Mat4 ret;
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j)
            ret(i, j) = rhs * lhs(i, j);
    return ret;
}

// Multiplies a float to every value in a matrix and returns the resulting matrix.
Mat4 operator*(float lhs, const Mat4& rhs)
{
    Mat4 ret;
    ret = rhs * lhs;
    return ret;
}

// ============= Stream operators =============

// Prints a nicely formatted 3x3 matrix to output stream
std::ostream& operator<<(std::ostream& os, const Mat4& mtx)
{
    os << std::fixed << std::setprecision(4);
    os << "-------------\n| ";

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            os << mtx(i, j);
            if (j != 3)
                os << " ";
            else
                os << " |\n";
        }
        if (i != 3)
            os << "| ";
    }
    os << "-------------";
    return os;
}
// ********************************* END Mat4 *********************************
} // namespace LE
