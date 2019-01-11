#ifndef CHROMA_MATH_HELPERS_H
#define CHROMA_MATH_HELPERS_H

/*
 * No user serviceable parts here.
 *
 * Don't use this file directly, instead include math/mat*.h
 */

#include <math.h>
#include <stdint.h>
#include <sys/types.h>
#include <algorithm>
#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#include "compiler.h"
#include "../quat.h"
#include "vector_helpers.h"

namespace math {
namespace details {
namespace matrix {

inline constexpr int transpose(int v) { return v; }
inline constexpr float transpose(float v) { return v; }
inline constexpr double transpose(double v) { return v; }

inline constexpr int trace(int v) { return v; }
inline constexpr float trace(float v) { return v; }
inline constexpr double trace(double v) { return v; }

template<typename MATRIX>
MATH_PURE
MATRIX 
gauss_jordan_inverse(const MATRIX& src) {
    typedef typename MATRIX::value_type T;
    static constexpr unsigned int N = MATRIX::NUM_ROWS;
    MATRIX tmp(src);
    MATRIX inverted(1);

    for (size_t i = 0; i < N; ++i) {
        // look for largest element in i'th column
        size_t swap = i;
        T t = std::abs(tmp[i][i]);
        for (size_t j = i + 1; j < N; ++j) {
            const T t2 = std::abs(tmp[j][i]);
            if (t2 > t) {
                swap = j;
                t = t2;
            }
        }

        if (swap != i) {
            // swap columns.
            std::swap(tmp[i], tmp[swap]);
            std::swap(inverted[i], inverted[swap]);
        }

        const T denom(tmp[i][i]);
        for (size_t k = 0; k < N; ++k) {
            tmp[i][k] /= denom;
            inverted[i][k] /= denom;
        }

        // factor out the lower triangle
        for (size_t j = 0; j < N; ++j) {
            if (j != i) {
                const T t = tmp[j][i];
                for (size_t k = 0; k < N; ++k) {
                    tmp[j][k] -= tmp[i][k] * t;
                    inverted[j][k] -= inverted[i][k] * t;
                }
            }
        }
    }

    return inverted;
}

// 2x2 matrix inverse is easy.
template <typename MATRIX>
MATH_PURE
MATRIX 
fast_inverse2(const MATRIX& x) {
    typedef typename MATRIX::value_type T;

    // Assuming the input matrix is:
    // | a b |
    // | c d |
    //
    // The analytic inverse is
    // | d -b |
    // | -c a | / (a d - b c)
    //
    // Importantly, our matrices are column-major!

    MATRIX inverted(MATRIX::NO_INIT);

    const T a = x[0][0];
    const T c = x[0][1];
    const T b = x[1][0];
    const T d = x[1][1];

    const T det((a * d) - (b * c));
    inverted[0][0] = d / det;
    inverted[0][1] = -c / det;
    inverted[1][0] = -b / det;
    inverted[1][1] = a / det;
    return inverted;
}


// From the Wikipedia article on matrix inversion's section on fast 3x3
// matrix inversion:
// http://en.wikipedia.org/wiki/Invertible_matrix#Inversion_of_3.C3.973_matrices
template <typename MATRIX>
MATH_PURE
MATRIX 
fast_inverse3(const MATRIX& x) {
    typedef typename MATRIX::value_type T;

    // Assuming the input matrix is:
    // | a b c |
    // | d e f |
    // | g h i |
    //
    // The analytic inverse is
    // | A B C |^T
    // | D E F |
    // | G H I | / determinant
    //
    // Which is
    // | A D G |
    // | B E H |
    // | C F I | / determinant
    //
    // Where:
    // A = (ei - fh), B = (fg - di), C = (dh - eg)
    // D = (ch - bi), E = (ai - cg), F = (bg - ah)
    // G = (bf - ce), H = (cd - af), I = (ae - bd)
    //
    // and the determinant is a*A + b*B + c*C (The rule of Sarrus)
    //
    // Importantly, our matrices are column-major!

    MATRIX inverted(MATRIX::NO_INIT);

    const T a = x[0][0];
    const T b = x[1][0];
    const T c = x[2][0];
    const T d = x[0][1];
    const T e = x[1][1];
    const T f = x[2][1];
    const T g = x[0][2];
    const T h = x[1][2];
    const T i = x[2][2];

    // Do the full analytic inverse
    const T A = e * i - f * h;
    const T B = f * g - d * i;
    const T C = d * h - e * g;
    inverted[0][0] = A;                 // A
    inverted[0][1] = B;                 // B
    inverted[0][2] = C;                 // C
    inverted[1][0] = c * h - b * i;     // D
    inverted[1][1] = a * i - c * g;     // E
    inverted[1][2] = b * g - a * h;     // F
    inverted[2][0] = b * f - c * e;     // G
    inverted[2][1] = c * d - a * f;     // H
    inverted[2][2] = a * e - b * d;     // I

    const T det(a * A + b * B + c * C);
    for (size_t col = 0; col < 3; ++col) {
        for (size_t row = 0; row < 3; ++row) {
            inverted[col][row] /= det;
        }
    }

    return inverted;
}


template <typename MATRIX>
inline constexpr MATH_PURE
MATRIX 
inverse(const MATRIX& matrix) {
    static_assert(MATRIX::NUM_ROWS == MATRIX::NUM_COLS, "only square matrices can be inverted");
    return (MATRIX::NUM_ROWS == 2) ? fast_inverse2<MATRIX>(matrix) :
        ((MATRIX::NUM_ROWS == 3) ? fast_inverse3<MATRIX>(matrix) :
         gauss_jordan_inverse<MATRIX>(matrix));
}

template<typename MATRIX_R, typename MATRIX_A, typename MATRIX_B>
MATH_PURE
MATRIX_R 
multiply(const MATRIX_A& lhs, const MATRIX_B& rhs) {
    // pre-requisite:
    //  lhs : D columns, R rows
    //  rhs : C columns, D rows
    //  res : C columns, R rows

    static_assert(MATRIX_A::NUM_COLS == MATRIX_B::NUM_ROWS,
                  "matrices can't be multiplied. invalid dimensions.");
    static_assert(MATRIX_R::NUM_COLS == MATRIX_B::NUM_COLS,
                  "invalid dimension of matrix multiply result.");
    static_assert(MATRIX_R::NUM_ROWS == MATRIX_A::NUM_ROWS,
                  "invalid dimension of matrix multiply result.");

    MATRIX_R res(MATRIX_R::NO_INIT);
    for (size_t col = 0; col < MATRIX_R::NUM_COLS; ++col) {
        res[col] = lhs * rhs[col];
    }
    return res;
}

template <typename MATRIX>
MATH_PURE
MATRIX  
transpose(const MATRIX& m) {
    // for now we only handle square matrix transpose
    static_assert(MATRIX::NUM_COLS == MATRIX::NUM_ROWS, "transpose only supports square matrices");
    MATRIX result(MATRIX::NO_INIT);
    for (size_t col = 0; col < MATRIX::NUM_COLS; ++col) {
        for (size_t row = 0; row < MATRIX::NUM_ROWS; ++row) {
            result[col][row] = transpose(m[row][col]);
        }
    }
    return result;
}

template <typename MATRIX>
MATH_PURE
typename MATRIX::value_type  
trace(const MATRIX& m) {
    static_assert(MATRIX::NUM_COLS == MATRIX::NUM_ROWS, "trace only defined for square matrices");
    typename MATRIX::value_type result(0);
    for (size_t col = 0; col < MATRIX::NUM_COLS; ++col) {
        result += trace(m[col][col]);
    }
    return result;
}

template <typename MATRIX>
MATH_PURE
typename MATRIX::col_type  
diag(const MATRIX& m) {
    static_assert(MATRIX::NUM_COLS == MATRIX::NUM_ROWS, "diag only defined for square matrices");
    typename MATRIX::col_type result;
    for (size_t col = 0; col < MATRIX::NUM_COLS; ++col) {
        result[col] = m[col][col];
    }
    return result;
}

// This is taken from the Imath MatrixAlgo code, and is identical to Eigen.
template <typename MATRIX>
Quaternion<typename MATRIX::value_type> 
extract_quat(const MATRIX& mat) {
    typedef typename MATRIX::value_type T;

    Quaternion<T> quat(Quaternion<T>::NO_INIT);

    // compute the trace to see if it is positive or not.
    const T trace = mat[0][0] + mat[1][1] + mat[2][2];

    // check the sign of the trace
    if (MATH_LIKELY(trace > 0)) {
        // trace is positive
        T s = std::sqrt(trace + 1);
        quat.w = T(0.5) * s;
        s = T(0.5) / s;
        quat.x = (mat[1][2] - mat[2][1]) * s;
        quat.y = (mat[2][0] - mat[0][2]) * s;
        quat.z = (mat[0][1] - mat[1][0]) * s;
    }
    else {
        // trace is negative

        // find the index of the greatest diagonal
        size_t i = 0;
        if (mat[1][1] > mat[0][0]) { i = 1; }
        if (mat[2][2] > mat[i][i]) { i = 2; }

        // get the next indices: (n+1)%3
        static constexpr size_t next_ijk[3] = { 1, 2, 0 };
        size_t j = next_ijk[i];
        size_t k = next_ijk[j];
        T s = std::sqrt((mat[i][i] - (mat[j][j] + mat[k][k])) + 1);
        quat[i] = T(0.5) * s;
        if (s != 0) {
            s = T(0.5) / s;
        }
        quat.w = (mat[j][k] - mat[k][j]) * s;
        quat[j] = (mat[i][j] + mat[j][i]) * s;
        quat[k] = (mat[i][k] + mat[k][i]) * s;
    }
    return quat;
}

}  // namespace matrix


template <template<typename T> class BASE, typename T>
class MatrixProductOperators {
public:
    BASE<T>& 
    operator*=(T v) {
        BASE<T>& lhs(static_cast<BASE<T>&>(*this));
        for (size_t col = 0; col < BASE<T>::NUM_COLS; ++col) {
            lhs[col] *= v;
        }
        return lhs;
    }

    template<typename U>
    const BASE<T>& 
    operator*=(const BASE<U>& rhs) {
        BASE<T>& lhs(static_cast<BASE<T>&>(*this));
        lhs = matrix::multiply<BASE<T> >(lhs, rhs);
        return lhs;
    }

    BASE<T>& 
    operator/=(T v) {
        BASE<T>& lhs(static_cast<BASE<T>&>(*this));
        for (size_t col = 0; col < BASE<T>::NUM_COLS; ++col) {
            lhs[col] /= v;
        }
        return lhs;
    }

    template<typename U>
    friend MATH_PURE
    BASE<T>  
    operator*(const BASE<T>& lhs, const BASE<U>& rhs) {
        return matrix::multiply<BASE<T> >(lhs, rhs);
    }
};


/*
 * MatrixSquareFunctions implements functions on a matrix of type BASE<T>.
 *
 * BASE only needs to implement:
 *  - operator[]
 *  - col_type
 *  - row_type
 *  - COL_SIZE
 *  - ROW_SIZE
 */
template<template<typename U> class BASE, typename T>
class MatrixSquareFunctions {
public:
    friend inline MATH_PURE 
    BASE<T>
    inverse(const BASE<T>& matrix) {
        return matrix::inverse(matrix);
    }

    friend inline MATH_PURE
    BASE<T>
    transpose(const BASE<T>& m) {
        return matrix::transpose(m);
    }

    friend inline MATH_PURE
    T 
    trace(const BASE<T>& m) {
        return matrix::trace(m);
    }
};


template<template<typename U> class BASE, typename T>
class MatrixHelpers {
public:
    constexpr inline size_t get_column_size() const { return BASE<T>::COL_SIZE; }
    constexpr inline size_t get_row_size() const { return BASE<T>::ROW_SIZE; }
    constexpr inline size_t get_column_count() const { return BASE<T>::NUM_COLS; }
    constexpr inline size_t get_row_count() const { return BASE<T>::NUM_ROWS; }
    constexpr inline size_t size()  const { return BASE<T>::ROW_SIZE; }  // for Vector*<>

    constexpr 
    T const* 
    as_array() const {
        return &static_cast<BASE<T> const &>(*this)[0][0];
    }

    inline constexpr
    T const&
    operator()(size_t row, size_t col) const {
        return static_cast<BASE<T> const &>(*this)[col][row];
    }

    inline 
    T&
    operator()(size_t row, size_t col) {
        return static_cast<BASE<T>&>(*this)[col][row];
    }

    friend inline MATH_PURE
    BASE<T>
    abs(BASE<T> m) {
        for (size_t col = 0; col < BASE<T>::NUM_COLS; ++col) {
            m[col] = abs(m[col]);
        }
        return m;
    }
};


template<template<typename U> class BASE, typename T>
class MatrixTransform {
public:
    inline constexpr 
    MatrixTransform() {
        static_assert(BASE<T>::NUM_ROWS == 3 || BASE<T>::NUM_ROWS == 4, "3x3 or 4x4 matrices only");
    }

    template <typename A, typename VEC>
    static 
    BASE<T> 
    rotate(A radian, const VEC& about) {
        BASE<T> r;
        T c = std::cos(radian);
        T s = std::sin(radian);
        if (about.x == 1 && about.y == 0 && about.z == 0) {
            r[1][1] = c;   r[2][2] = c;
            r[1][2] = s;   r[2][1] = -s;
        }
        else if (about.x == 0 && about.y == 1 && about.z == 0) {
            r[0][0] = c;   r[2][2] = c;
            r[2][0] = s;   r[0][2] = -s;
        }
        else if (about.x == 0 && about.y == 0 && about.z == 1) {
            r[0][0] = c;   r[1][1] = c;
            r[0][1] = s;   r[1][0] = -s;
        }
        else {
            VEC nabout = normalize(about);
            typename VEC::value_type x = nabout.x;
            typename VEC::value_type y = nabout.y;
            typename VEC::value_type z = nabout.z;
            T nc = 1 - c;
            T xy = x * y;
            T yz = y * z;
            T zx = z * x;
            T xs = x * s;
            T ys = y * s;
            T zs = z * s;
            r[0][0] = x * x*nc + c;    r[1][0] = xy * nc - zs;    r[2][0] = zx * nc + ys;
            r[0][1] = xy * nc + zs;    r[1][1] = y * y*nc + c;    r[2][1] = yz * nc - xs;
            r[0][2] = zx * nc - ys;    r[1][2] = yz * nc + xs;    r[2][2] = z * z*nc + c;

            // Clamp results to -1, 1.
            for (size_t col = 0; col < 3; ++col) {
                for (size_t row = 0; row < 3; ++row) {
                    r[col][row] = std::min(std::max(r[col][row], T(-1)), T(1));
                }
            }
        }
        return r;
    }

    /**
     * Create a matrix from euler angles using YPR around YXZ respectively
     * @param yaw about Y axis
     * @param pitch about X axis
     * @param roll about Z axis
     */
    template <typename Y, typename P, typename R,
        typename = typename std::enable_if<std::is_arithmetic<Y>::value >::type,
        typename = typename std::enable_if<std::is_arithmetic<P>::value >::type,
        typename = typename std::enable_if<std::is_arithmetic<R>::value >::type>
    static 
    BASE<T> 
    euler_yxz(Y yaw, P pitch, R roll) {
        return euler_zyx(roll, pitch, yaw);
    }

    /**
     * Create a matrix from euler angles using YPR around ZYX respectively
     * @param roll about X axis
     * @param pitch about Y axis
     * @param yaw about Z axis
     *
     * The euler angles are applied in ZYX order. i.e: a vector is first rotated
     * about X (roll) then Y (pitch) and then Z (yaw).
     */
    template <typename Y, typename P, typename R,
        typename = typename std::enable_if<std::is_arithmetic<Y>::value >::type,
        typename = typename std::enable_if<std::is_arithmetic<P>::value >::type,
        typename = typename std::enable_if<std::is_arithmetic<R>::value >::type>
    static 
    BASE<T>
    euler_zyx(Y yaw, P pitch, R roll) {
        BASE<T> r;
        T cy = std::cos(yaw);
        T sy = std::sin(yaw);
        T cp = std::cos(pitch);
        T sp = std::sin(pitch);
        T cr = std::cos(roll);
        T sr = std::sin(roll);
        T cc = cr * cy;
        T cs = cr * sy;
        T sc = sr * cy;
        T ss = sr * sy;
        r[0][0] = cp * cy;
        r[0][1] = cp * sy;
        r[0][2] = -sp;
        r[1][0] = sp * sc - cs;
        r[1][1] = sp * ss + cc;
        r[1][2] = cp * sr;
        r[2][0] = sp * cc + ss;
        r[2][1] = sp * cs - sc;
        r[2][2] = cp * cr;

        // Clamp results to -1, 1.
        for (size_t col = 0; col < 3; ++col) {
            for (size_t row = 0; row < 3; ++row) {
                r[col][row] = std::min(std::max(r[col][row], T(-1)), T(1));
            }
        }
        return r;
    }

    Quaternion<T> to_quaternion() const {
        return matrix::extract_quat(static_cast<const BASE<T>&>(*this));
    }
};


template <template<typename T> class BASE, typename T>
class MatrixDebug {
public:
    friend 
    std::ostream& 
    operator<<(std::ostream& stream, const BASE<T>& m) {
        for (size_t row = 0; row < BASE<T>::NUM_ROWS; ++row) {
            if (row != 0) {
                stream << std::endl;
            }
            if (row == 0) {
                stream << "/ ";
            }
            else if (row == BASE<T>::NUM_ROWS - 1) {
                stream << "\\ ";
            }
            else {
                stream << "| ";
            }
            for (size_t col = 0; col < BASE<T>::NUM_COLS; ++col) {
                stream << std::setw(10) << std::to_string(m[col][row]);
            }
            if (row == 0) {
                stream << " \\";
            }
            else if (row == BASE<T>::NUM_ROWS - 1) {
                stream << " /";
            }
            else {
                stream << " |";
            }
        }
        return stream;
    }
};

}  // namespace details
}  // namespace math

#endif