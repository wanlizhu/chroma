#ifndef CHROMA_MATH_MAT3_H
#define CHROMA_MATH_MAT3_H

#include "quat.h"
#include "details/matrix_helpers.h"
#include "vec3.h"
#include "details/compiler.h"
#include <limits.h>
#include <stdint.h>
#include <sys/types.h>

namespace math {
namespace details {

template<typename T>
class MATH_EMPTY_BASES Matrix33 :
    public GPUDeviceCompatible,
    public VectorUnaryOperators<Matrix33, T>,
    public VectorComparisonOperators<Matrix33, T>,
    public VectorAddOperators<Matrix33, T>,
    public MatrixProductOperators<Matrix33, T>,
    public MatrixSquareFunctions<Matrix33, T>,
    public MatrixTransform<Matrix33, T>,
    public MatrixHelpers<Matrix33, T>,
    public MatrixDebug<Matrix33, T> {
public:
    enum no_init { NO_INIT };
    typedef T value_type;
    typedef T& reference;
    typedef T const& const_reference;
    typedef size_t size_type;
    typedef Vector3<T> col_type;
    typedef Vector3<T> row_type;

    static constexpr size_t COL_SIZE = col_type::SIZE;  // size of a column (i.e.: number of rows)
    static constexpr size_t ROW_SIZE = row_type::SIZE;  // size of a row (i.e.: number of columns)
    static constexpr size_t NUM_ROWS = COL_SIZE;
    static constexpr size_t NUM_COLS = ROW_SIZE;

private:
    /*
     *  <--  N columns  -->
     *
     *  a[0][0] a[1][0] a[2][0] ... a[N][0]    ^
     *  a[0][1] a[1][1] a[2][1] ... a[N][1]    |
     *  a[0][2] a[1][2] a[2][2] ... a[N][2]  M rows
     *  ...                                    |
     *  a[0][M] a[1][M] a[2][M] ... a[N][M]    v
     *
     *  COL_SIZE = M
     *  ROW_SIZE = N
     *  m[0] = [ a[0][0] a[0][1] a[0][2] ... a[0][M] ]
     */
    col_type m_value[NUM_COLS];

public:
    inline constexpr 
    col_type const&
    operator[](size_t column) const {
        assert(column < NUM_COLS);
        return m_value[column];
    }

    inline constexpr
    col_type&
    operator[](size_t column) {
        assert(column < NUM_COLS);
        return m_value[column];
    }

    constexpr explicit 
    Matrix33(no_init) {}
    
    constexpr
    Matrix33();
    
    template<typename U>
    constexpr explicit
    Matrix33(U v);
    
    template<typename U>
    constexpr explicit
    Matrix33(const Vector3<U>& v);
    
    template<typename U>
    constexpr explicit 
    Matrix33(const Matrix33<U>& rhs);
    
    template<typename A, typename B, typename C>
    constexpr
    Matrix33(const Vector3<A>& v0, const Vector3<B>& v1, const Vector3<C>& v2);
    
    template<typename A, typename B, typename C,
              typename D, typename E, typename F,
              typename G, typename H, typename I>
    constexpr explicit
    Matrix33(A m00, B m01, C m02,
             D m10, E m11, F m12,
             G m20, H m21, I m22);

    struct row_major_init {
        template<typename A, typename B, typename C,
                 typename D, typename E, typename F,
                 typename G, typename H, typename I>
        constexpr explicit 
        row_major_init(A m00, B m01, C m02,
                       D m10, E m11, F m12,
                       G m20, H m21, I m22) noexcept
            : m(m00, m10, m20,
                m01, m11, m21,
                m02, m12, m22) {}
    private:
        friend Matrix33;
        Matrix33 m;
    };
    constexpr explicit
    Matrix33(row_major_init c)
        : Matrix33(std::move(c.m)) { }
    
    template<typename U>
    constexpr explicit
    Matrix33(const Quaternion<U>& q);


    // orthogonalize only works on matrices of size 3x3.
    friend inline constexpr
    Matrix33 orthogonalize(const Matrix33& m) {
        Matrix33 ret(Matrix33::NO_INIT);
        ret[0] = normalize(m[0]);
        ret[2] = normalize(cross(ret[0], m[1]));
        ret[1] = normalize(cross(ret[2], ret[0]));
        return ret;
    }

    /**
     * Packs the tangent frame represented by the specified matrix into a quaternion.
     * Reflection is preserved by encoding it as the sign of the w component in the
     * resulting quaternion. Since -0 cannot always be represented on the GPU, this
     * function computes a bias to ensure values are always either positive or negative,
     * never 0. The bias is computed based on the specified storageSize, which defaults
     * to 2 bytes, making the resulting quaternion suitable for storage into an SNORM16
     * vector.
     */
    static constexpr 
    Quaternion<T> 
    pack_tangent_frame(const Matrix33& m, size_t storageSize = sizeof(int16_t));

    template<typename A>
    static constexpr 
    Matrix33
    translate(const Vector3<A>& t) {
        Matrix33 r;
        r[2] = t;
        return r;
    }

    template<typename A>
    static constexpr
    Matrix33
    translate(A t) {
        Matrix33 r;
        r[2] = Vector3<T>{ t };
        return r;
    }

    template<typename A>
    static constexpr
    Matrix33 
    scale(const Vector3<A>& s) {
        return Matrix33{ s };
    }

    template<typename A>
    static constexpr 
    Matrix33
    scale(A s) {
        return Matrix33{ Vector3<T>{ s } };
    }
};

template<typename T>
constexpr
Matrix33<T>::Matrix33() {
    m_value[0] = col_type(1, 0, 0);
    m_value[1] = col_type(0, 1, 0);
    m_value[2] = col_type(0, 0, 1);
}

template<typename T>
template<typename U>
constexpr 
Matrix33<T>::Matrix33(U v) {
    m_value[0] = col_type(v, 0, 0);
    m_value[1] = col_type(0, v, 0);
    m_value[2] = col_type(0, 0, v);
}

template<typename T>
template<typename U>
constexpr
Matrix33<T>::Matrix33(const Vector3<U>& v) {
    m_value[0] = col_type(v.x, 0, 0);
    m_value[1] = col_type(0, v.y, 0);
    m_value[2] = col_type(0, 0, v.z);
}

template<typename T>
template<typename A, typename B, typename C,
          typename D, typename E, typename F,
          typename G, typename H, typename I>
constexpr 
Matrix33<T>::Matrix33(A m00, B m01, C m02,
                      D m10, E m11, F m12,
                      G m20, H m21, I m22) {
    m_value[0] = col_type(m00, m01, m02);
    m_value[1] = col_type(m10, m11, m12);
    m_value[2] = col_type(m20, m21, m22);
}

template<typename T>
template<typename U>
constexpr 
Matrix33<T>::Matrix33(const Matrix33<U>& rhs) {
    for (size_t col = 0; col < NUM_COLS; ++col) {
        m_value[col] = col_type(rhs[col]);
    }
}

template<typename T>
template<typename A, typename B, typename C>
constexpr 
Matrix33<T>::Matrix33(const Vector3<A>& v0, const Vector3<B>& v1, const Vector3<C>& v2) {
    m_value[0] = v0;
    m_value[1] = v1;
    m_value[2] = v2;
}

template<typename T>
template<typename U>
constexpr 
Matrix33<T>::Matrix33(const Quaternion<U>& q) {
    const U n = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
    const U s = n > 0 ? 2 / n : 0;
    const U x = s * q.x;
    const U y = s * q.y;
    const U z = s * q.z;
    const U xx = x * q.x;
    const U xy = x * q.y;
    const U xz = x * q.z;
    const U xw = x * q.w;
    const U yy = y * q.y;
    const U yz = y * q.z;
    const U yw = y * q.w;
    const U zz = z * q.z;
    const U zw = z * q.w;
    m_value[0] = col_type(1 - yy - zz, xy + zw, xz - yw);  // NOLINT
    m_value[1] = col_type(xy - zw, 1 - xx - zz, yz + xw);  // NOLINT
    m_value[2] = col_type(xz + yw, yz - xw, 1 - xx - yy);  // NOLINT
}

 // matrix * column-vector, result is a vector of the same type than the input vector
template<typename T, typename U>
constexpr MATH_PURE
typename Matrix33<U>::col_type  
operator*(const Matrix33<T>& lhs, const Vector3<U>& rhs) {
    // Result is initialized to zero.
    typename Matrix33<U>::col_type result = {};
    for (size_t col = 0; col < Matrix33<T>::NUM_COLS; ++col) {
        result += lhs[col] * rhs[col];
    }
    return result;
}

// row-vector * matrix, result is a vector of the same type than the input vector
template<typename T, typename U>
constexpr MATH_PURE
typename Matrix33<U>::row_type  
operator*(const Vector3<U>& lhs, const Matrix33<T>& rhs) {
    typename Matrix33<U>::row_type result;
    for (size_t col = 0; col < Matrix33<T>::NUM_COLS; ++col) {
        result[col] = dot(lhs, rhs[col]);
    }
    return result;
}

// matrix * scalar, result is a matrix of the same type than the input matrix
template<typename T, typename U>
constexpr MATH_PURE
typename std::enable_if<std::is_arithmetic<U>::value, Matrix33<T>>::type 
operator*(Matrix33<T> lhs, U rhs) {
    return lhs *= rhs;
}

// scalar * matrix, result is a matrix of the same type than the input matrix
template<typename T, typename U>
constexpr MATH_PURE
typename std::enable_if<std::is_arithmetic<U>::value, Matrix33<T>>::type 
operator*(U lhs, const Matrix33<T>& rhs) {
    return rhs * lhs;
}


template<typename T>
constexpr 
Quaternion<T>
Matrix33<T>::pack_tangent_frame(const Matrix33<T>& m, size_t storageSize) {
    Quaternion<T> q = Matrix33<T>{ m[0], cross(m[2], m[0]), m[2] }.to_quaternion();
    q = positive(normalize(q));

    // Ensure w is never 0.0
    // Bias is 2^(nb_bits - 1) - 1
    const T bias = T(1.0) / T((1 << (storageSize * CHAR_BIT - 1)) - 1);
    if (q.w < bias) {
        q.w = bias;

        const T factor = (T)(std::sqrt(1.0 - (double)bias * (double)bias));
        q.xyz *= factor;
    }

    // If there's a reflection ((n x t) . b <= 0), make sure w is negative
    if (dot(cross(m[0], m[2]), m[1]) < T(0)) {
        q = -q;
    }

    return q;
}

template<typename T>
constexpr MATH_PURE
typename Matrix33<T>::col_type  
diag(const Matrix33<T>& m) {
    return matrix::diag(m);
}

}  // namespace details


typedef details::Matrix33<double> mat3;
typedef details::Matrix33<float> mat3f;

}  // namespace math


namespace std {

template<typename T>
constexpr 
void
swap(math::details::Matrix33<T>& lhs, math::details::Matrix33<T>& rhs) noexcept {
    const T t00 = lhs[0][0];
    const T t01 = lhs[0][1];
    const T t02 = lhs[0][2];
    const T t10 = lhs[1][0];
    const T t11 = lhs[1][1];
    const T t12 = lhs[1][2];
    const T t20 = lhs[2][0];
    const T t21 = lhs[2][1];
    const T t22 = lhs[2][2];

    lhs[0][0] = rhs[0][0];
    lhs[0][1] = rhs[0][1];
    lhs[0][2] = rhs[0][2];
    lhs[1][0] = rhs[1][0];
    lhs[1][1] = rhs[1][1];
    lhs[1][2] = rhs[1][2];
    lhs[2][0] = rhs[2][0];
    lhs[2][1] = rhs[2][1];
    lhs[2][2] = rhs[2][2];

    rhs[0][0] = t00;
    rhs[0][1] = t01;
    rhs[0][2] = t02;
    rhs[1][0] = t10;
    rhs[1][1] = t11;
    rhs[1][2] = t12;
    rhs[2][0] = t20;
    rhs[2][1] = t21;
    rhs[2][2] = t22;
}
} // namespace std

#endif