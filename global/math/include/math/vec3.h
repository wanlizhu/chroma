#ifndef CHROMA_MATH_VEC3_H
#define CHROMA_MATH_VEC3_H

#include "vec2.h"
#include "half.h"
#include <stdint.h>
#include <sys/types.h>

namespace math {
namespace details {

template <typename T>
class MATH_EMPTY_BASES Vector3 :
    public VectorProductOperators<Vector3, T>,
    public VectorAddOperators<Vector3, T>,
    public VectorUnaryOperators<Vector3, T>,
    public VectorComparisonOperators<Vector3, T>,
    public VectorFunctions<Vector3, T>,
    public VectorDebug<Vector3, T> {
public:
    typedef T value_type;
    typedef T& reference;
    typedef T const& const_reference;
    typedef size_t size_type;
    static constexpr size_t SIZE = 3;

    union {
        T v[SIZE];
        Vector2<T> xy;
        Vector2<T> st;
        Vector2<T> rg;
        struct {
            union {
                T x;
                T s;
                T r;
            };
            union {
                struct { T y, z; };
                struct { T t, p; };
                struct { T g, b; };
                Vector2<T> yz;
                Vector2<T> tp;
                Vector2<T> gb;
            };
        };
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

    constexpr Vector3() = default;

    template<typename A, typename = typename std::enable_if<std::is_arithmetic<A>::value>::type>
    constexpr 
    Vector3(A v) 
        : x(v), y(v), z(v) {}

    template<typename A, typename B, typename C>
    constexpr 
    Vector3(A x, B y, C z)
        : x(x), y(y), z(z) {}

    template<typename A, typename B>
    constexpr
    Vector3(const Vector2<A>& v, B z)
        : x(v.x), y(v.y), z(z) {}

    template<typename A>
    explicit constexpr 
    Vector3(const Vector3<A>& v) 
        : x(v.x), y(v.y), z(v.z) {}

    template <typename RT>
    friend inline constexpr
    Vector3 
    cross(const Vector3& u, const Vector3<RT>& v) {
        return Vector3(
            u.y*v.z - u.z*v.y,
            u.z*v.x - u.x*v.z,
            u.x*v.y - u.y*v.x);
    }
};

} // namespace details

typedef details::Vector3<double> double3;
typedef details::Vector3<float> float3;
typedef details::Vector3<half> half3;
typedef details::Vector3<int32_t> int3;
typedef details::Vector3<uint32_t> uint3;
typedef details::Vector3<int16_t> short3;
typedef details::Vector3<uint16_t> ushort3;
typedef details::Vector3<int8_t> byte3;
typedef details::Vector3<uint8_t> ubyte3;
typedef details::Vector3<bool> bool3;

} // namespace math

#endif