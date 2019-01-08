#ifndef CHROMA_MATH_QUATERNION_H
#define CHROMA_MATH_QUATERNION_H

/*
 * No user serviceable parts here.
 *
 * Don't use this file directly, instead include math/quat.h
 */

#include <math.h>
#include <stdint.h>
#include <sys/types.h>
#include <iostream>

#include "compiler.h"
#include "../vec3.h"

namespace math {
namespace details {

/*
 * QuaternionProductOperators implements basic arithmetic and basic compound assignment
 * operators on a quaternion of type BASE<T>.
 *
 * BASE only needs to implement operator[] and size().
 * By simply inheriting from QuaternionProductOperators<BASE, T> BASE will automatically
 * get all the functionality here.
 */
template <template<typename T> class QUATERNION, typename T>
class QuaternionProductOperators {
public:
    // compound assignment from a another quaternion of the same size but different element type.
    template <typename OTHER>
    constexpr QUATERNION<T>& operator *= (const QUATERNION<OTHER>& r) {
        QUATERNION<T>& q = static_cast<QUATERNION<T>&>(*this);
        q = q * r;
        return q;
    }

    // compound assignment products by a scalar
    constexpr QUATERNION<T>& operator *= (T v) {
        QUATERNION<T>& lhs = static_cast<QUATERNION<T>&>(*this);
        for (size_t i = 0; i < QUATERNION<T>::size(); i++) {
            lhs[i] *= v;
        }
        return lhs;
    }
    constexpr QUATERNION<T>& operator /= (T v) {
        QUATERNION<T>& lhs = static_cast<QUATERNION<T>&>(*this);
        for (size_t i = 0; i < QUATERNION<T>::size(); i++) {
            lhs[i] /= v;
        }
        return lhs;
    }

    /*
     * NOTE: the functions below ARE NOT member methods. They are friend functions
     * with they definition inlined with their declaration. This makes these
     * template functions available to the compiler when (and only when) this class
     * is instantiated, at which point they're only templated on the 2nd parameter
     * (the first one, BASE<T> being known).
     */

     /* The operators below handle operation between quaternion of the same size
      * but of a different element type.
      */
    template<typename RT>
    friend inline
        constexpr QUATERNION<T> MATH_PURE operator *(const QUATERNION<T>& q, const QUATERNION<RT>& r) {
        // could be written as:
        //  return QUATERNION<T>(
        //            q.w*r.w - dot(q.xyz, r.xyz),
        //            q.w*r.xyz + r.w*q.xyz + cross(q.xyz, r.xyz));

        return QUATERNION<T>(
            q.w*r.w - q.x*r.x - q.y*r.y - q.z*r.z,
            q.w*r.x + q.x*r.w + q.y*r.z - q.z*r.y,
            q.w*r.y - q.x*r.z + q.y*r.w + q.z*r.x,
            q.w*r.z + q.x*r.y - q.y*r.x + q.z*r.w);
    }

    template<typename RT>
    friend inline
        constexpr Vector3<T> MATH_PURE operator *(const QUATERNION<T>& q, const Vector3<RT>& v) {
        // note: if q is known to be a unit quaternion, then this simplifies to:
        //  Vector3<T> t = 2 * cross(q.xyz, v)
        //  return v + (q.w * t) + cross(q.xyz, t)
        return imaginary(q * QUATERNION<T>(v, 0) * inverse(q));
    }


    /* For quaternions, we use explicit "by a scalar" products because it's much faster
     * than going (implicitly) through the quaternion multiplication.
     * For reference: we could use the code below instead, but it would be a lot slower.
     *  friend inline
     *  constexpr BASE<T> MATH_PURE operator *(const BASE<T>& q, const BASE<T>& r) {
     *      return BASE<T>(
     *              q.w*r.w - q.x*r.x - q.y*r.y - q.z*r.z,
     *              q.w*r.x + q.x*r.w + q.y*r.z - q.z*r.y,
     *              q.w*r.y - q.x*r.z + q.y*r.w + q.z*r.x,
     *              q.w*r.z + q.x*r.y - q.y*r.x + q.z*r.w);
     *
     */
    friend inline
        constexpr QUATERNION<T> MATH_PURE operator *(QUATERNION<T> q, T scalar) {
        // don't pass q by reference because we need a copy anyways
        return q *= scalar;
    }
    friend inline
        constexpr QUATERNION<T> MATH_PURE operator *(T scalar, QUATERNION<T> q) {
        // don't pass q by reference because we need a copy anyways
        return q *= scalar;
    }

    friend inline
        constexpr QUATERNION<T> MATH_PURE operator /(QUATERNION<T> q, T scalar) {
        // don't pass q by reference because we need a copy anyways
        return q /= scalar;
    }
};


/*
 * QuaternionFunctions implements functions on a quaternion of type BASE<T>.
 *
 * BASE only needs to implement operator[] and size().
 * By simply inheriting from QuaternionFunctions<BASE, T> BASE will automatically
 * get all the functionality here.
 */
template <template<typename T> class QUATERNION, typename T>
class QuaternionFunctions {
public:
    /*
     * NOTE: the functions below ARE NOT member methods. They are friend functions
     * with they definition inlined with their declaration. This makes these
     * template functions available to the compiler when (and only when) this class
     * is instantiated, at which point they're only templated on the 2nd parameter
     * (the first one, BASE<T> being known).
     */
    template<typename RT>
    friend inline
        constexpr T MATH_PURE dot(const QUATERNION<T>& p, const QUATERNION<RT>& q) {
        return p.x * q.x +
            p.y * q.y +
            p.z * q.z +
            p.w * q.w;
    }

    friend inline
        T MATH_PURE norm(const QUATERNION<T>& q) {
        return std::sqrt(dot(q, q));
    }

    friend inline
        T MATH_PURE length(const QUATERNION<T>& q) {
        return norm(q);
    }

    friend inline
        constexpr T MATH_PURE length2(const QUATERNION<T>& q) {
        return dot(q, q);
    }

    friend inline
        constexpr QUATERNION<T> MATH_PURE normalize(const QUATERNION<T>& q) {
        return length(q) ? q / length(q) : QUATERNION<T>(static_cast<T>(1));
    }

    friend inline
        constexpr QUATERNION<T> MATH_PURE conj(const QUATERNION<T>& q) {
        return QUATERNION<T>(q.w, -q.x, -q.y, -q.z);
    }

    friend inline
        constexpr QUATERNION<T> MATH_PURE inverse(const QUATERNION<T>& q) {
        return conj(q) * (1 / dot(q, q));
    }

    friend inline
        constexpr T MATH_PURE real(const QUATERNION<T>& q) {
        return q.w;
    }

    friend inline
        constexpr Vector3<T> MATH_PURE imaginary(const QUATERNION<T>& q) {
        return q.xyz;
    }

    friend inline
        constexpr QUATERNION<T> MATH_PURE unreal(const QUATERNION<T>& q) {
        return QUATERNION<T>(q.xyz, 0);
    }

    friend inline
        constexpr QUATERNION<T> MATH_PURE cross(const QUATERNION<T>& p, const QUATERNION<T>& q) {
        return unreal(p*q);
    }

    friend inline
        QUATERNION<T> MATH_PURE exp(const QUATERNION<T>& q) {
        const T nq(norm(q.xyz));
        return std::exp(q.w)*QUATERNION<T>((sin(nq) / nq)*q.xyz, std::cos(nq));
    }

    friend inline
        QUATERNION<T> MATH_PURE log(const QUATERNION<T>& q) {
        const T nq(norm(q));
        return QUATERNION<T>((std::acos(q.w / nq) / norm(q.xyz))*q.xyz, std::log(nq));
    }

    friend inline
        QUATERNION<T> MATH_PURE pow(const QUATERNION<T>& q, T a) {
        // could also be computed as: exp(a*log(q));
        const T nq(norm(q));
        const T theta(a*std::acos(q.w / nq));
        return std::pow(nq, a) * QUATERNION<T>(normalize(q.xyz) * std::sin(theta), std::cos(theta));
    }

    friend inline
        QUATERNION<T> MATH_PURE slerp(const QUATERNION<T>& p, const QUATERNION<T>& q, T t) {
        // could also be computed as: pow(q * inverse(p), t) * p;
        const T d = dot(p, q);
        const T npq = std::sqrt(dot(p, p) * dot(q, q));  // ||p|| * ||q||
        const T a = std::acos(std::abs(d) / npq);
        const T a0 = a * (1 - t);
        const T a1 = a * t;
        const T isina = 1 / sin(a);
        const T s0 = std::sin(a0) * isina;
        const T s1 = std::sin(a1) * isina;
        // ensure we're taking the "short" side
        return normalize(s0 * p + ((d < 0) ? (-s1) : (s1)) * q);
    }

    friend inline
        constexpr QUATERNION<T> MATH_PURE lerp(const QUATERNION<T>& p, const QUATERNION<T>& q, T t) {
        return ((1 - t) * p) + (t * q);
    }

    friend inline
        constexpr QUATERNION<T> MATH_PURE nlerp(const QUATERNION<T>& p, const QUATERNION<T>& q, T t) {
        return normalize(lerp(p, q, t));
    }

    friend inline
        constexpr QUATERNION<T> MATH_PURE positive(const QUATERNION<T>& q) {
        return q.w < 0 ? -q : q;
    }
};


/*
 * QuaternionDebug implements functions on a vector of type BASE<T>.
 *
 * BASE only needs to implement operator[] and size().
 * By simply inheriting from QuaternionDebug<BASE, T> BASE will automatically
 * get all the functionality here.
 */
template <template<typename T> class QUATERNION, typename T>
class QuaternionDebug {
public:
    /*
     * NOTE: the functions below ARE NOT member methods. They are friend functions
     * with they definition inlined with their declaration. This makes these
     * template functions available to the compiler when (and only when) this class
     * is instantiated, at which point they're only templated on the 2nd parameter
     * (the first one, BASE<T> being known).
     */
    friend std::ostream& operator<< (std::ostream& stream, const QUATERNION<T>& q) {
        return stream << "< " << q.w << " + " << q.x << "i + " << q.y << "j + " << q.z << "k >";
    }
};

}  // namespace details
}  // namespace math

#endif