#ifndef CHROMA_MATH_VEC2_H
#define CHROMA_MATH_VEC2_H

#include "details/vector_helpers.h"
#include "half.h"
#include <assert.h>
#include <stdint.h>
#include <sys/types.h>
#include <type_traits>

namespace math {
namespace details {

template <typename T>
class MATH_EMPTY_BASES Vector2 :
    public VectorProductOperators<Vector2, T>,
    public VectorAddOperators<Vector2, T>,
    public VectorUnaryOperators<Vector2, T>,
    public VectorComparisonOperators<Vector2, T>,
    public VectorFunctions<Vector2, T>,
    public VectorDebug<Vector2, T> {
public:
    typedef T value_type;
    typedef T& reference;
    typedef T const& const_reference;
    typedef size_t size_type;
    static constexpr size_t SIZE = 2;

    union {
        T v[SIZE];
        struct { T x, y; };
        struct { T s, t; };
        struct { T r, g; };
    };

    inline constexpr 
    size_type
    size() const {
        return SIZE;
    }

    inline constexpr 
    T const&
    operator[](size_t i) const {
        assert(i < SIZE);
        return v[i];
    }

    inline constexpr 
    T&
    operator[](size_t i) {
        assert(i < SIZE);
        return v[i];
    }

    constexpr Vector2() = default;

    template<typename A, typename = typename std::enable_if<std::is_arithmetic<A>::value>::type>
    constexpr
    Vector2(A v)
        : x(v), y(v) {}

    template<typename A, typename B>
    constexpr
    Vector2(A x, B y) 
        : x(x), y(y) {}

    template<typename A>
    explicit constexpr
    Vector2(const Vector2<A>& v) 
        : x(v.x), y(v.y) {}

    template <typename RT>
    friend inline constexpr
    value_type
    cross(const Vector2& u, const Vector2<RT>& v) {
        return value_type(u.x*v.y - u.y*v.x);
    }
};
} // namespace details

typedef details::Vector2<double> double2;
typedef details::Vector2<float> float2;
typedef details::Vector2<half> half2;
typedef details::Vector2<int32_t> int2;
typedef details::Vector2<uint32_t> uint2;
typedef details::Vector2<int16_t> short2;
typedef details::Vector2<uint16_t> ushort2;
typedef details::Vector2<int8_t> byte2;
typedef details::Vector2<uint8_t> ubyte2;
typedef details::Vector2<bool> bool2;

} // namespace math

#endif