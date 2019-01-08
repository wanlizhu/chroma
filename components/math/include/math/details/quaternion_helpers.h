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

template <template<typename T> class QUATERNION, typename T>
class QuaternionProductOperators {
public:
    template<typename OTHER>
    constexpr
    QUATERNION<T>& 
    operator*=(const QUATERNION<OTHER>& r) {
        QUATERNION<T>& q = static_cast<QUATERNION<T>&>(*this);
        q = q * r;
        return q;
    }

    constexpr 
    QUATERNION<T>&
    operator*=(T v) {
        QUATERNION<T>& lhs = static_cast<QUATERNION<T>&>(*this);
        for (size_t i = 0; i < QUATERNION<T>::size(); i++) {
            lhs[i] *= v;
        }
        return lhs;
    }

    constexpr
    QUATERNION<T>&
    operator/=(T v) {
        QUATERNION<T>& lhs = static_cast<QUATERNION<T>&>(*this);
        for (size_t i = 0; i < QUATERNION<T>::size(); i++) {
            lhs[i] /= v;
        }
        return lhs;
    }

    template<typename RT>
    friend inline constexpr MATH_PURE
    QUATERNION<T>  
    operator*(const QUATERNION<T>& q, const QUATERNION<RT>& r) {
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
    friend inline constexpr MATH_PURE
    Vector3<T> 
    operator*(const QUATERNION<T>& q, const Vector3<RT>& v) {
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
    friend inline constexpr MATH_PURE
    QUATERNION<T>  
    operator*(QUATERNION<T> q, T scalar) {
        return q *= scalar;
    }

    friend inline constexpr MATH_PURE
    QUATERNION<T>
    operator*(T scalar, QUATERNION<T> q) {
        return q *= scalar;
    }

    friend inline constexpr MATH_PURE
    QUATERNION<T>
    operator/(QUATERNION<T> q, T scalar) {
        return q /= scalar;
    }
};


template <template<typename T> class QUATERNION, typename T>
class QuaternionFunctions {
public:
    template<typename RT>
    friend inline constexpr MATH_PURE
    T
    dot(const QUATERNION<T>& p, const QUATERNION<RT>& q) {
        return p.x * q.x +
            p.y * q.y +
            p.z * q.z +
            p.w * q.w;
    }

    friend inline MATH_PURE
    T
    norm(const QUATERNION<T>& q) {
        return std::sqrt(dot(q, q));
    }

    friend inline MATH_PURE
    T
    length(const QUATERNION<T>& q) {
        return norm(q);
    }

    friend inline constexpr MATH_PURE
    T
    length2(const QUATERNION<T>& q) {
        return dot(q, q);
    }

    friend inline constexpr MATH_PURE
    QUATERNION<T>
    normalize(const QUATERNION<T>& q) {
        return length(q) ? q / length(q) : QUATERNION<T>(static_cast<T>(1));
    }

    friend inline constexpr MATH_PURE
    QUATERNION<T>
    conj(const QUATERNION<T>& q) {
        return QUATERNION<T>(q.w, -q.x, -q.y, -q.z);
    }

    friend inline constexpr MATH_PURE
    QUATERNION<T>
    inverse(const QUATERNION<T>& q) {
        return conj(q) * (1 / dot(q, q));
    }

    friend inline constexpr MATH_PURE
    T
    real(const QUATERNION<T>& q) {
        return q.w;
    }

    friend inline constexpr MATH_PURE
    Vector3<T>  
    imaginary(const QUATERNION<T>& q) {
        return q.xyz;
    }

    friend inline constexpr MATH_PURE
    QUATERNION<T>
    unreal(const QUATERNION<T>& q) {
        return QUATERNION<T>(q.xyz, 0);
    }

    friend inline constexpr MATH_PURE
    QUATERNION<T> 
    cross(const QUATERNION<T>& p, const QUATERNION<T>& q) {
        return unreal(p*q);
    }

    friend inline MATH_PURE
    QUATERNION<T>
    exp(const QUATERNION<T>& q) {
        const T nq(norm(q.xyz));
        return std::exp(q.w)*QUATERNION<T>((sin(nq) / nq)*q.xyz, std::cos(nq));
    }

    friend inline MATH_PURE
    QUATERNION<T>
    log(const QUATERNION<T>& q) {
        const T nq(norm(q));
        return QUATERNION<T>((std::acos(q.w / nq) / norm(q.xyz))*q.xyz, std::log(nq));
    }

    friend inline MATH_PURE
    QUATERNION<T>
    pow(const QUATERNION<T>& q, T a) {
        // could also be computed as: exp(a*log(q));
        const T nq(norm(q));
        const T theta(a*std::acos(q.w / nq));
        return std::pow(nq, a) * QUATERNION<T>(normalize(q.xyz) * std::sin(theta), std::cos(theta));
    }

    friend inline MATH_PURE
    QUATERNION<T>
    slerp(const QUATERNION<T>& p, const QUATERNION<T>& q, T t) {
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

    friend inline constexpr MATH_PURE
    QUATERNION<T> 
    lerp(const QUATERNION<T>& p, const QUATERNION<T>& q, T t) {
        return ((1 - t) * p) + (t * q);
    }

    friend inline constexpr MATH_PURE
    QUATERNION<T>
    nlerp(const QUATERNION<T>& p, const QUATERNION<T>& q, T t) {
        return normalize(lerp(p, q, t));
    }

    friend inline constexpr MATH_PURE
    QUATERNION<T>
    positive(const QUATERNION<T>& q) {
        return q.w < 0 ? -q : q;
    }
};


template <template<typename T> class QUATERNION, typename T>
class QuaternionDebug {
public:
    friend
    std::ostream& 
    operator<< (std::ostream& stream, const QUATERNION<T>& q) {
        return stream << "< " << q.w << " + " << q.x << "i + " << q.y << "j + " << q.z << "k >";
    }
};

}  // namespace details
}  // namespace math

#endif