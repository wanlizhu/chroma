#ifndef CHROMA_NUMERIC_MAT2_H
#define CHROMA_NUMERIC_MAT2_H

#include "details/matrix_helpers.h"
#include "details/compiler.h"
#include <numeric/vec2.h>
#include <stdint.h>
#include <sys/types.h>

namespace numeric {
namespace details {

    template<typename T>
    class NUM_EMPTY_BASES Matrix22 :
        public VectorUnaryOperators<Matrix22, T>,
        public VectorComparisonOperators<Matrix22, T>,
        public VectorAddOperators<Matrix22, T>,
        public MatrixProductOperators<Matrix22, T>,
        public MatrixSquareFunctions<Matrix22, T>,
        public MatrixHelpers<Matrix22, T>,
        public MatrixDebug<Matrix22, T> {
    public:
        enum no_init { NO_INIT };
        typedef T value_type;
        typedef T& reference;
        typedef T const& const_reference;
        typedef size_t size_type;
        typedef Vector2<T> col_type;
        typedef Vector2<T> row_type;

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
        inline 
        col_type& 
        operator[](size_t column) {
            assert(column < NUM_COLS);
            return m_value[column];
        }

        constexpr explicit 
        Matrix22(no_init) {}

        constexpr 
        Matrix22();

        template<typename U>
        constexpr explicit 
        Matrix22(U v);
    
        template<typename U>
        constexpr explicit
        Matrix22(const Vector2<U>& v);

        template<typename U>
        constexpr explicit 
        Matrix22(const Matrix22<U>& rhs);

        template<typename A, typename B>
        constexpr 
        Matrix22(const Vector2<A>& v0, const Vector2<B>& v1);

        template<typename A, typename B, typename C, typename D>
        constexpr explicit 
        Matrix22(A m00, B m01, C m10, D m11);

        struct row_major_init {
            template<typename A, typename B, typename C, typename D>
            constexpr explicit 
            row_major_init(A m00, B m01, C m10, D m11) noexcept
                : m(m00, m10, m01, m11) {}
        private:
            friend Matrix22;
            Matrix22 m;
        };
        constexpr explicit 
        Matrix22(row_major_init c) 
            : Matrix22(std::move(c.m)) { }


        // rotate by radians in the 2D plane
        static 
        Matrix22<T> 
        rotate(T radian) {
            Matrix22<T> r(Matrix22<T>::NO_INIT);
            T c = std::cos(radian);
            T s = std::sin(radian);
            r[0][0] = c;   r[1][1] = c;
            r[0][1] = s;   r[1][0] = -s;
            return r;
        }

        // returns false if the two matrices are different. May return false if they're the same,
        // with some elements only differing by +0 or -0.
        // Behaviour is undefined with NaNs.
        static constexpr 
        bool
        fuzzy_equal(Matrix22 l, Matrix22 r) noexcept {
            uint64_t const* const li = reinterpret_cast<uint64_t const*>(&l);
            uint64_t const* const ri = reinterpret_cast<uint64_t const*>(&r);
            uint64_t result = 0;

            for (size_t i = 0; i < sizeof(Matrix22) / sizeof(uint64_t); i++) {
                result |= li[i] ^ ri[i];
            }
            return result != 0;
        }

        template<typename A>
        static constexpr
        Matrix22
        translate(const Vector2<A>& t) {
            Matrix22 r;
            r[2] = t;
            return r;
        }

        template<typename A>
        static constexpr
        Matrix22
        translate(A t) {
            Matrix22 r;
            r[1] = Vector2<T>{ t };
            return r;
        }

        template<typename A>
        static constexpr
        Matrix22
        scale(const Vector2<A>& s) {
            return Matrix22{ s };
        }

        template<typename A>
        static constexpr 
        Matrix22 
        scale(A s) {
            return Matrix22{ Vector2<T>{ s, s } };
        }
    };


    // Since the matrix code could become pretty big quickly, we don't inline most operations.
    template<typename T>
    constexpr 
    Matrix22<T>::Matrix22() {
        m_value[0] = col_type(1, 0);
        m_value[1] = col_type(0, 1);
    }

    template<typename T>
    template<typename U>
    constexpr 
    Matrix22<T>::Matrix22(U v) {
        m_value[0] = col_type(v, 0);
        m_value[1] = col_type(0, v);
    }

    template<typename T>
    template<typename U>
    constexpr 
    Matrix22<T>::Matrix22(const Vector2<U>& v) {
        m_value[0] = col_type(v.x, 0);
        m_value[1] = col_type(0, v.y);
    }

    // Note that the arrangement of values in the constructor is the transpose of the matrix notation.
    template<typename T>
    template<typename A, typename B, typename C, typename D>
    constexpr
    Matrix22<T>::Matrix22(A m00, B m01, C m10, D m11) {
        m_value[0] = col_type(m00, m01);
        m_value[1] = col_type(m10, m11);
    }

    template<typename T>
    template<typename U>
    constexpr 
    Matrix22<T>::Matrix22(const Matrix22<U>& rhs) {
        for (size_t col = 0; col < NUM_COLS; ++col) {
            m_value[col] = col_type(rhs[col]);
        }
    }

    template<typename T>
    template<typename A, typename B>
    constexpr
    Matrix22<T>::Matrix22(const Vector2<A>& v0, const Vector2<B>& v1) {
        m_value[0] = v0;
        m_value[1] = v1;
    }

     // matrix * column-vector, result is a vector of the same type than the input vector
    template<typename T, typename U>
    constexpr NUM_PURE
    typename Matrix22<U>::col_type  
    operator*(const Matrix22<T>& lhs, const Vector2<U>& rhs) {
        // result is initialized to zero.
        typename Matrix22<U>::col_type result = {};
        for (size_t col = 0; col < Matrix22<T>::NUM_COLS; ++col) {
            result += lhs[col] * rhs[col];
        }
        return result;
    }

    // row-vector * matrix, result is a vector of the same type than the input vector
    template<typename T, typename U>
    constexpr NUM_PURE
    typename Matrix22<U>::row_type  
    operator*(const Vector2<U>& lhs, const Matrix22<T>& rhs) {
        typename Matrix22<U>::row_type result;
        for (size_t col = 0; col < Matrix22<T>::NUM_COLS; ++col) {
            result[col] = dot(lhs, rhs[col]);
        }
        return result;
    }

    // matrix * scalar, result is a matrix of the same type than the input matrix
    template<typename T, typename U>
    constexpr NUM_PURE
    typename std::enable_if<std::is_arithmetic<U>::value, Matrix22<T>>::type 
    operator*(Matrix22<T> lhs, U rhs) {
        return lhs *= rhs;
    }

    // scalar * matrix, result is a matrix of the same type than the input matrix
    template<typename T, typename U>
    constexpr NUM_PURE
    typename std::enable_if<std::is_arithmetic<U>::value, Matrix22<T>>::type 
    operator*(U lhs, const Matrix22<T>& rhs) {
        return rhs * lhs;
    }

    // FIXME: this should go into MatrixSquareFunctions<> but for some reason
    // BASE<T>::col_type is not accessible from there (???)
    template<typename T>
    constexpr NUM_PURE
    typename Matrix22<T>::col_type  
    diag(const Matrix22<T>& m) {
        return matrix::diag(m);
    }

}  // namespace details

    typedef details::Matrix22<double> mat2;
    typedef details::Matrix22<float> mat2f;

}  // namespace numeric


namespace std {

template<typename T>
constexpr 
void
swap(numeric::details::Matrix22<T>& lhs, numeric::details::Matrix22<T>& rhs) noexcept {
    //    numeric::details::Matrix22<T> t(lhs);
    //    lhs = rhs;
    //    rhs = t;

    const T t00 = lhs[0][0];
    const T t01 = lhs[0][1];
    const T t10 = lhs[1][0];
    const T t11 = lhs[1][1];

    lhs[0][0] = rhs[0][0];
    lhs[0][1] = rhs[0][1];
    lhs[1][0] = rhs[1][0];
    lhs[1][1] = rhs[1][1];

    rhs[0][0] = t00;
    rhs[0][1] = t01;
    rhs[1][0] = t10;
    rhs[1][1] = t11;
}

} // namespace std

#endif
