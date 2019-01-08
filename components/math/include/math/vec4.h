#ifndef CHROMA_MATH_VEC4_H
#define CHROMA_MATH_VEC4_H

#include "vec3.h"
#include "half.h"
#include <stdint.h>
#include <sys/types.h>

namespace math {
namespace details {

template <typename T>
class MATH_EMPTY_BASES Vector4 :
    public VectorProductOperators<Vector4, T>,
    public VectorAddOperators<Vector4, T>,
    public VectorUnaryOperators<Vector4, T>,
    public VectorComparisonOperators<Vector4, T>,
    public VectorFunctions<Vector4, T>,
    public VectorDebug<Vector4, T> {
public:
    typedef T value_type;
    typedef T& reference;
    typedef T const& const_reference;
    typedef size_t size_type;
    static constexpr size_t SIZE = 4;

    union {
        T v[SIZE];
        Vector2<T> xy, st, rg;
        Vector3<T> xyz, stp, rgb;
        struct {
            union { T x, s, r; };
            union {
                Vector2<T> yz, tp, gb;
                Vector3<T> yzw, tpq, gba;
                struct {
                    union { T y, t, g; };
                    union {
                        Vector2<T> zw, pq, ba;
                        struct { T z, w; };
                        struct { T p, q; };
                        struct { T b, a; };
                    };
                };
            };
        };
    };

    inline constexpr size_type size() const { return SIZE; }
    inline constexpr T const& operator[](size_t i) const {
        // only possible in C++0x14 with constexpr
        assert(i < SIZE);
        return v[i];
    }
    inline constexpr T& operator[](size_t i) {
        assert(i < SIZE);
        return v[i];
    }


    // default constructor
    constexpr Vector4() = default;

    // handles implicit conversion to a tvec4. must not be explicit.
    template<typename A, typename = typename std::enable_if<std::is_arithmetic<A>::value >::type>
    constexpr Vector4(A v) : x(v), y(v), z(v), w(v) { }

    template<typename A, typename B, typename C, typename D>
    constexpr Vector4(A x, B y, C z, D w) : x(x), y(y), z(z), w(w) { }

    template<typename A, typename B, typename C>
    constexpr Vector4(const Vector2<A>& v, B z, C w) : x(v.x), y(v.y), z(z), w(w) { }

    template<typename A, typename B>
    constexpr Vector4(const Vector3<A>& v, B w) : x(v.x), y(v.y), z(v.z), w(w) { }

    template<typename A>
    explicit constexpr Vector4(const Vector4<A>& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
};

}  // namespace details

typedef details::Vector4<double> double4;
typedef details::Vector4<float> float4;
typedef details::Vector4<half> half4;
typedef details::Vector4<int32_t> int4;
typedef details::Vector4<uint32_t> uint4;
typedef details::Vector4<int16_t> short4;
typedef details::Vector4<uint16_t> ushort4;
typedef details::Vector4<int8_t> byte4;
typedef details::Vector4<uint8_t> ubyte4;
typedef details::Vector4<bool> bool4;

}  // namespace math

#endif
