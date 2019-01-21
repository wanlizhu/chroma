#ifndef CHROMA_MATH_MAT4_H
#define CHROMA_MATH_MAT4_H

#include "mat3.h"
#include "quat.h"
#include "details/matrix_helpers.h"
#include "vec3.h"
#include "vec4.h"
#include "details/compiler.h"

#include <stdint.h>
#include <sys/types.h>
#include <limits>

namespace math {
namespace details {

template<typename T>
class Quaternion;

template<typename T>
class MATH_EMPTY_BASES Matrix44 :
    public VectorUnaryOperators<Matrix44, T>,
    public VectorComparisonOperators<Matrix44, T>,
    public VectorAddOperators<Matrix44, T>,
    public MatrixProductOperators<Matrix44, T>,
    public MatrixSquareFunctions<Matrix44, T>,
    public MatrixTransform<Matrix44, T>,
    public MatrixHelpers<Matrix44, T>,
    public MatrixDebug<Matrix44, T> {
public:
    enum no_init { NO_INIT };
    typedef T value_type;
    typedef T& reference;
    typedef T const& const_reference;
    typedef size_t size_type;
    typedef Vector4<T> col_type;
    typedef Vector4<T> row_type;

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
    Matrix44(no_init) {}
    
    constexpr
    Matrix44();
    
    template<typename U>
    constexpr explicit
    Matrix44(U v);
    
    template<typename U>
    constexpr explicit
    Matrix44(const Vector4<U>& v);
    
    template<typename U>
    constexpr explicit
    Matrix44(const Matrix44<U>& rhs);
    
    template<typename A, typename B, typename C, typename D>
    constexpr
    Matrix44(const Vector4<A>& v0, const Vector4<B>& v1, const Vector4<C>& v2, const Vector4<D>& v3);
    
    template<typename A, typename B, typename C, typename D,
              typename E, typename F, typename G, typename H,
              typename I, typename J, typename K, typename L,
              typename M, typename N, typename O, typename P>
    constexpr explicit
    Matrix44(A m00, B m01, C m02, D m03,
             E m10, F m11, G m12, H m13,
             I m20, J m21, K m22, L m23,
             M m30, N m31, O m32, P m33);

    // row major initialize.
    struct row_major_init {
        template<typename A, typename B, typename C, typename D,
                 typename E, typename F, typename G, typename H,
                 typename I, typename J, typename K, typename L,
                 typename M, typename N, typename O, typename P>
        constexpr explicit 
        row_major_init(A m00, B m01, C m02, D m03,
                       E m10, F m11, G m12, H m13,
                       I m20, J m21, K m22, L m23,
                       M m30, N m31, O m32, P m33) noexcept
            : m(m00, m10, m20, m30,
                m01, m11, m21, m31,
                m02, m12, m22, m32,
                m03, m13, m23, m33) {}
    private:
        friend Matrix44;
        Matrix44 m;
    };
    constexpr explicit
    Matrix44(row_major_init c)
        : Matrix44(std::move(c.m)) {}

    template<typename U>
    constexpr explicit 
    Matrix44(const Quaternion<U>& q);

    template<typename U>
    constexpr explicit
    Matrix44(const Matrix33<U>& matrix);

    template<typename U, typename V>
    constexpr 
    Matrix44(const Matrix33<U>& matrix, const Vector3<V>& translation);

    template<typename U, typename V>
    constexpr
    Matrix44(const Matrix33<U>& matrix, const Vector4<V>& column3);

    static constexpr
    bool
    fuzzy_equal(Matrix44 const& l, Matrix44 const& r) noexcept {
        uint64_t const* const li = reinterpret_cast<uint64_t const*>(&l);
        uint64_t const* const ri = reinterpret_cast<uint64_t const*>(&r);
        uint64_t result = 0;
        // for some reason clang is not able to vectorize this loop when the number of iteration
        // is known and constant (!?!?!). Still this is better than operator==.
        for (size_t i = 0; i < sizeof(Matrix44) / sizeof(uint64_t); i++) {
            result |= li[i] ^ ri[i];
        }
        return result != 0;
    }

    static constexpr
    Matrix44
    ortho(T left, T right, T bottom, T top, T near, T far);

    static constexpr
    Matrix44
    frustum(T left, T right, T bottom, T top, T near, T far);

    enum class Fov {
        HORIZONTAL,
        VERTICAL
    };
    static constexpr 
    Matrix44
    perspective(T fov, T aspect, T near, T far, Fov direction = Fov::VERTICAL);

    template<typename A, typename B, typename C>
    static constexpr
    Matrix44
    look_at(const Vector3<A>& eye, const Vector3<B>& center, const Vector3<C>& up);

    template<typename A>
    static constexpr
    Vector3<A>
    project(const Matrix44& projectionMatrix, Vector3<A> vertice) {
        Vector4<A> r = projectionMatrix * Vector4<A>{ vertice, 1 };
        return r.xyz * (1 / r.w);
    }

    template<typename A>
    static constexpr
    Vector4<A>
    project(const Matrix44& projectionMatrix, Vector4<A> vertice) {
        vertice = projectionMatrix * vertice;
        return { vertice.xyz * (1 / vertice.w), 1 };
    }

    inline constexpr 
    Matrix33<T>
    upper_left() const {
        return Matrix33<T>(m_value[0].xyz, m_value[1].xyz, m_value[2].xyz);
    }

    template<typename A>
    static constexpr
    Matrix44
    translate(const Vector3<A>& t) {
        Matrix44 r;
        r[3] = Vector4<T>{ t, 1 };
        return r;
    }

    template<typename A>
    static constexpr 
    Matrix44
    translate(A t) {
        Matrix44 r;
        r[3] = Vector4<T>{ t, t, t, 1 };
        return r;
    }

    template<typename A>
    static constexpr
    Matrix44
    scale(const Vector3<A>& s) {
        return Matrix44{ Vector4<T>{ s, 1 } };
    }

    template<typename A>
    static constexpr 
    Matrix44
    scale(A s) {
        return Matrix44{ Vector4<T>{ s, s, s, 1 } };
    }
};


template<typename T>
constexpr
Matrix44<T>::Matrix44() {
    m_value[0] = col_type(1.0f, 0.0f, 0.0f, 0.0f);
    m_value[1] = col_type(0.0f, 1.0f, 0.0f, 0.0f);
    m_value[2] = col_type(0.0f, 0.0f, 1.0f, 0.0f);
    m_value[3] = col_type(0.0f, 0.0f, 0.0f, 1.0f);
}

template<typename T>
template<typename U>
constexpr
Matrix44<T>::Matrix44(U v) {
    m_value[0] = col_type(v, 0, 0, 0);
    m_value[1] = col_type(0, v, 0, 0);
    m_value[2] = col_type(0, 0, v, 0);
    m_value[3] = col_type(0, 0, 0, v);
}

template<typename T>
template<typename U>
constexpr
Matrix44<T>::Matrix44(const Vector4<U>& v) {
    m_value[0] = col_type(v.x, 0, 0, 0);
    m_value[1] = col_type(0, v.y, 0, 0);
    m_value[2] = col_type(0, 0, v.z, 0);
    m_value[3] = col_type(0, 0, 0, v.w);
}

template<typename T>
template<typename A, typename B, typename C, typename D,
         typename E, typename F, typename G, typename H,
         typename I, typename J, typename K, typename L,
         typename M, typename N, typename O, typename P>
constexpr 
Matrix44<T>::Matrix44(A m00, B m01, C m02, D m03,
                      E m10, F m11, G m12, H m13,
                      I m20, J m21, K m22, L m23,
                      M m30, N m31, O m32, P m33) {
    m_value[0] = col_type(m00, m01, m02, m03);
    m_value[1] = col_type(m10, m11, m12, m13);
    m_value[2] = col_type(m20, m21, m22, m23);
    m_value[3] = col_type(m30, m31, m32, m33);
}

template<typename T>
template<typename U>
constexpr
Matrix44<T>::Matrix44(const Matrix44<U>& rhs) {
    for (size_t col = 0; col < NUM_COLS; ++col) {
        m_value[col] = col_type(rhs[col]);
    }
}

template<typename T>
template<typename A, typename B, typename C, typename D>
constexpr 
Matrix44<T>::Matrix44(const Vector4<A>& v0, const Vector4<B>& v1,
                      const Vector4<C>& v2, const Vector4<D>& v3) {
    m_value[0] = col_type(v0);
    m_value[1] = col_type(v1);
    m_value[2] = col_type(v2);
    m_value[3] = col_type(v3);
}

template<typename T>
template<typename U>
constexpr 
Matrix44<T>::Matrix44(const Quaternion<U>& q) {
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
    m_value[0] = col_type(1 - yy - zz, xy + zw, xz - yw, 0);
    m_value[1] = col_type(xy - zw, 1 - xx - zz, yz + xw, 0);  // NOLINT
    m_value[2] = col_type(xz + yw, yz - xw, 1 - xx - yy, 0);  // NOLINT
    m_value[3] = col_type(0, 0, 0, 1);  // NOLINT
}

template<typename T>
template<typename U>
constexpr 
Matrix44<T>::Matrix44(const Matrix33<U>& m) {
    m_value[0] = col_type(m[0][0], m[0][1], m[0][2], 0);
    m_value[1] = col_type(m[1][0], m[1][1], m[1][2], 0);
    m_value[2] = col_type(m[2][0], m[2][1], m[2][2], 0);
    m_value[3] = col_type(0, 0, 0, 1);  // NOLINT
}

template<typename T>
template<typename U, typename V>
constexpr 
Matrix44<T>::Matrix44(const Matrix33<U>& m, const Vector3<V>& v) {
    m_value[0] = col_type(m[0][0], m[0][1], m[0][2], 0);
    m_value[1] = col_type(m[1][0], m[1][1], m[1][2], 0);
    m_value[2] = col_type(m[2][0], m[2][1], m[2][2], 0);
    m_value[3] = col_type(v[0], v[1], v[2], 1);  // NOLINT
}

template<typename T>
template<typename U, typename V>
constexpr 
Matrix44<T>::Matrix44(const Matrix33<U>& m, const Vector4<V>& v) {
    m_value[0] = col_type(m[0][0], m[0][1], m[0][2], 0);  // NOLINT
    m_value[1] = col_type(m[1][0], m[1][1], m[1][2], 0);  // NOLINT
    m_value[2] = col_type(m[2][0], m[2][1], m[2][2], 0);  // NOLINT
    m_value[3] = col_type(v[0], v[1], v[2], v[3]);  // NOLINT
}

template<typename T>
constexpr 
Matrix44<T>
Matrix44<T>::ortho(T left, T right, T bottom, T top, T near, T far) {
    Matrix44<T> m;
    m[0][0] = 2 / (right - left);
    m[1][1] = 2 / (top - bottom);
    m[2][2] = -2 / (far - near);
    m[3][0] = -(right + left) / (right - left);
    m[3][1] = -(top + bottom) / (top - bottom);
    m[3][2] = -(far + near) / (far - near);
    return m;
}

template<typename T>
constexpr
Matrix44<T>
Matrix44<T>::frustum(T left, T right, T bottom, T top, T near, T far) {
    Matrix44<T> m;
    m[0][0] = (2 * near) / (right - left);
    m[1][1] = (2 * near) / (top - bottom);
    m[2][0] = (right + left) / (right - left);
    m[2][1] = (top + bottom) / (top - bottom);
    m[2][2] = -(far + near) / (far - near);
    m[2][3] = -1;
    m[3][2] = -(2 * far * near) / (far - near);
    m[3][3] = 0;
    return m;
}

template<typename T>
constexpr
Matrix44<T>
Matrix44<T>::perspective(T fov, T aspect, T near, T far, Matrix44::Fov direction) {
    T h;
    T w;

    if (direction == Matrix44::Fov::VERTICAL) {
        h = std::tan(fov * M_PI / 360.0f) * near;
        w = h * aspect;
    }
    else {
        w = std::tan(fov * M_PI / 360.0f) * near;
        h = w / aspect;
    }
    return frustum(-w, w, -h, h, near, far);
}

/*
 * Returns a matrix representing the pose of a virtual camera looking towards -Z in its
 * local Y-up coordinate system. "eye" is where the camera is located, "center" is the points its
 * looking at and "up" defines where the Y axis of the camera's local coordinate system is.
 */
template<typename T>
template<typename A, typename B, typename C>
constexpr
Matrix44<T>
Matrix44<T>::look_at(const Vector3<A>& eye, const Vector3<B>& center, const Vector3<C>& up) {
    Vector3<T> z_axis(normalize(center - eye));
    Vector3<T> norm_up(normalize(up));
    if (std::abs(dot(z_axis, norm_up)) > T(0.999)) {
        // Fix up vector if we're degenerate (looking straight up, basically)
        norm_up = { norm_up.z, norm_up.x, norm_up.y };
    }
    Vector3<T> x_axis(normalize(cross(z_axis, norm_up)));
    Vector3<T> y_axis(cross(x_axis, z_axis));
    return Matrix44<T>(Vector4<T>(x_axis, 0),
                       Vector4<T>(y_axis, 0),
                       Vector4<T>(-z_axis, 0),
                       Vector4<T>(eye, 1));
}

 // matrix * column-vector, result is a vector of the same type than the input vector
template<typename T, typename U>
constexpr MATH_PURE
typename Matrix44<T>::col_type 
operator*(const Matrix44<T>& lhs, const Vector4<U>& rhs) {
    // Result is initialized to zero.
    typename Matrix44<T>::col_type result = {};
    for (size_t col = 0; col < Matrix44<T>::NUM_COLS; ++col) {
        result += lhs[col] * T(rhs[col]);
    }
    return result;
}

// mat44 * vec3, result is vec3( mat44 * {vec3, 1} )
template<typename T, typename U>
constexpr MATH_PURE
typename Matrix44<T>::col_type 
operator*(const Matrix44<T>& lhs, const Vector3<U>& rhs) {
    return lhs * Vector4<U>{ rhs, 1 };
}


// row-vector * matrix, result is a vector of the same type than the input vector
template<typename T, typename U>
constexpr MATH_PURE
typename Matrix44<U>::row_type 
operator*(const Vector4<U>& lhs, const Matrix44<T>& rhs) {
    typename Matrix44<U>::row_type result;
    for (size_t col = 0; col < Matrix44<T>::NUM_COLS; ++col) {
        result[col] = dot(lhs, rhs[col]);
    }
    return result;
}

// matrix * scalar, result is a matrix of the same type than the input matrix
template<typename T, typename U>
constexpr MATH_PURE
typename std::enable_if<std::is_arithmetic<U>::value, Matrix44<T>>::type
operator*(Matrix44<T> lhs, U rhs) {
    return lhs *= rhs;
}

// scalar * matrix, result is a matrix of the same type than the input matrix
template<typename T, typename U>
constexpr MATH_PURE
typename std::enable_if<std::is_arithmetic<U>::value, Matrix44<T>>::type 
operator*(U lhs, const Matrix44<T>& rhs) {
    return rhs * lhs;
}

template<typename T>
constexpr MATH_PURE
typename Matrix44<T>::col_type 
diag(const Matrix44<T>& m) {
    return matrix::diag(m);
}

} // namespace details

typedef details::Matrix44<double> mat4;
typedef details::Matrix44<float> mat4f;

}  // namespace math


namespace std {
template<typename T>
constexpr
void
swap(math::details::Matrix44<T>& lhs, math::details::Matrix44<T>& rhs) noexcept {
    const T t00 = lhs[0][0];
    const T t01 = lhs[0][1];
    const T t02 = lhs[0][2];
    const T t03 = lhs[0][3];
    const T t10 = lhs[1][0];
    const T t11 = lhs[1][1];
    const T t12 = lhs[1][2];
    const T t13 = lhs[1][3];
    const T t20 = lhs[2][0];
    const T t21 = lhs[2][1];
    const T t22 = lhs[2][2];
    const T t23 = lhs[2][3];
    const T t30 = lhs[3][0];
    const T t31 = lhs[3][1];
    const T t32 = lhs[3][2];
    const T t33 = lhs[3][3];

    lhs[0][0] = rhs[0][0];
    lhs[0][1] = rhs[0][1];
    lhs[0][2] = rhs[0][2];
    lhs[0][3] = rhs[0][3];
    lhs[1][0] = rhs[1][0];
    lhs[1][1] = rhs[1][1];
    lhs[1][2] = rhs[1][2];
    lhs[1][3] = rhs[1][3];
    lhs[2][0] = rhs[2][0];
    lhs[2][1] = rhs[2][1];
    lhs[2][2] = rhs[2][2];
    lhs[2][3] = rhs[2][3];
    lhs[3][0] = rhs[3][0];
    lhs[3][1] = rhs[3][1];
    lhs[3][2] = rhs[3][2];
    lhs[3][3] = rhs[3][3];

    rhs[0][0] = t00;
    rhs[0][1] = t01;
    rhs[0][2] = t02;
    rhs[0][3] = t03;
    rhs[1][0] = t10;
    rhs[1][1] = t11;
    rhs[1][2] = t12;
    rhs[1][3] = t13;
    rhs[2][0] = t20;
    rhs[2][1] = t21;
    rhs[2][2] = t22;
    rhs[2][3] = t23;
    rhs[3][0] = t30;
    rhs[3][1] = t31;
    rhs[3][2] = t32;
    rhs[3][3] = t33;
}
} // namespace std

#endif
