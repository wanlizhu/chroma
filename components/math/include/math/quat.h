#ifndef CHROMA_MATH_QUAT_H
#define CHROMA_MATH_QUAT_H

#include "half.h"
#include "details/quaternion_helpers.h"
#include "vec3.h"
#include "vec4.h"
#include "details/compiler.h"

#include <stdint.h>
#include <sys/types.h>

namespace math {
namespace details {

template <typename T>
class MATH_EMPTY_BASES Quaternion :
    public VectorAddOperators<Quaternion, T>,
    public VectorUnaryOperators<Quaternion, T>,
    public VectorComparisonOperators<Quaternion, T>,
    public QuaternionProductOperators<Quaternion, T>,
    public QuaternionFunctions<Quaternion, T>,
    public QuaternionDebug<Quaternion, T> {
public:
    enum no_init { NO_INIT };
    typedef T value_type;
    typedef T& reference;
    typedef T const& const_reference;
    typedef size_t size_type;

    /*
     * quaternion internals stored as:
     *
     * q = w + xi + yj + zk
     *
     *  q[0] = x;
     *  q[1] = y;
     *  q[2] = z;
     *  q[3] = w;
     *
     */
    union {
        struct { T x, y, z, w; };
        Vector4<T> xyzw;
        Vector3<T> xyz;
        Vector2<T> xy;
    };

    enum { SIZE = 4 };
    inline constexpr static size_type size() { return SIZE; }
    inline constexpr T const& operator[](size_t i) const {
        // only possible in C++0x14 with constexpr
        assert(i < SIZE);
        return (&x)[i];
    }
    inline constexpr T& operator[](size_t i) {
        assert(i < SIZE);
        return (&x)[i];
    }


    // we want the compiler generated versions for these...
    Quaternion(const Quaternion&) = default;
    ~Quaternion() = default;
    Quaternion& operator = (const Quaternion&) = default;


    // leaves object uninitialized. use with caution.
    explicit constexpr Quaternion(no_init) {}
    // default constructor. sets all values to zero.
    constexpr Quaternion() : x(0), y(0), z(0), w(0) { }
    // handles implicit conversion to a tvec4. must not be explicit.
    template<typename A>
    constexpr Quaternion(A w) : x(0), y(0), z(0), w(w) {
        static_assert(std::is_arithmetic<A>::value, "requires arithmetic type");
    }
    // initialize from 4 values to w + xi + yj + zk
    template<typename A, typename B, typename C, typename D>
    constexpr Quaternion(A w, B x, C y, D z) : x(x), y(y), z(z), w(w) { }
    // initialize from a vec3 + a value to : v.xi + v.yj + v.zk + w
    template<typename A, typename B>
    constexpr Quaternion(const Vector3<A>& v, B w) : x(v.x), y(v.y), z(v.z), w(w) { }
    // initialize from a double4
    template<typename A>
    constexpr explicit Quaternion(const Vector4<A>& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
    // initialize from a quaternion of a different type
    template<typename A>
    constexpr explicit Quaternion(const Quaternion<A>& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
    // conjugate operator
    constexpr Quaternion operator~() const {
        return conj(*this);
    }
    // constructs a quaternion from an axis and angle
    template <typename A, typename B>
    constexpr static Quaternion MATH_PURE from_axis_angle(const Vector3<A>& axis, B angle) {
        return Quaternion(std::sin(angle*0.5) * normalize(axis), std::cos(angle*0.5));
    }
};

}  // namespace details

typedef details::Quaternion<double> quat;
typedef details::Quaternion<float> quatf;
typedef details::Quaternion<half> quath;

constexpr inline quat operator"" _i(long double v) {
    return quat(0.0, double(v), 0.0, 0.0);
}
constexpr inline quat operator"" _j(long double v) {
    return quat(0.0, 0.0, double(v), 0.0);
}
constexpr inline quat operator"" _k(long double v) {
    return quat(0.0, 0.0, 0.0, double(v));
}

constexpr inline quat operator"" _i(unsigned long long v) {
    return quat(0.0, double(v), 0.0, 0.0);
}
constexpr inline quat operator"" _j(unsigned long long v) {
    return quat(0.0, 0.0, double(v), 0.0);
}
constexpr inline quat operator"" _k(unsigned long long v) {
    return quat(0.0, 0.0, 0.0, double(v));
}

}  // namespace math

#endif