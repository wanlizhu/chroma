#ifndef CHROMA_VECTOR_HELPERS_H
#define CHROMA_VECTOR_HELPERS_H

/*
 * No user serviceable parts here.
 *
 * Don't use this file directly, instead include math/vec{2|3|4}.h
 */

#include <math.h>
#include <stdint.h>
#include <sys/types.h>
#include <cmath>
#include <functional>
#include <limits>
#include <iostream>
#include <numeric>
#include <limits>
#include "compiler.h"
#include "../scalar.h"

namespace numeric {
namespace details {

/*
 * VectorAddOperators implements basic arithmetic and basic compound assignments operators.
 * Child class only needs to implement operator[] and size().
 */
template<template<typename T> class VECTOR, typename T>
class VectorAddOperators {
public:
    template<typename OTHER>
    constexpr 
    VECTOR<T>& 
    operator+=(const VECTOR<OTHER>& v) {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++) {
            lhs[i] += v[i];
        }
        return lhs;
    }
    template<typename OTHER>
    constexpr
    VECTOR<T>& 
    operator-=(const VECTOR<OTHER>& v) {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++) {
            lhs[i] -= v[i];
        }
        return lhs;
    }

    constexpr
    VECTOR<T>&
    operator+=(const VECTOR<T>& v) {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++) {
            lhs[i] += v[i];
        }
        return lhs;
    }
    constexpr 
    VECTOR<T>& 
    operator-=(const VECTOR<T>& v) {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++) {
            lhs[i] -= v[i];
        }
        return lhs;
    }

    template<typename RT>
    friend inline constexpr NUM_PURE
    VECTOR<T>  
    operator+(VECTOR<T> lv, const VECTOR<RT>& rv) {
        return lv += rv; 
    }
    template<typename RT>
    friend inline constexpr NUM_PURE
    VECTOR<T>  
    operator-(VECTOR<T> lv, const VECTOR<RT>& rv) {
        return lv -= rv;
    }

    // These handle operations like "vector + scalar" and "scalar + vector" by implicitly converting a scalar to a vector.
    friend inline constexpr NUM_PURE
    VECTOR<T>  
    operator+(VECTOR<T> lv, const VECTOR<T>& rv) {
        return lv += rv;
    }
    friend inline constexpr NUM_PURE
    VECTOR<T>  
    operator-(VECTOR<T> lv, const VECTOR<T>& rv) {
        return lv -= rv;
    }
};


template<template<typename T> class VECTOR, typename T>
class VectorProductOperators {
public:
    template<typename OTHER>
    constexpr 
    VECTOR<T>& 
    operator*=(const VECTOR<OTHER>& v) {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++) {
            lhs[i] *= v[i];
        }
        return lhs;
    }
    template<typename OTHER>
    constexpr 
    VECTOR<T>& 
    operator/=(const VECTOR<OTHER>& v) {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++) {
            lhs[i] /= v[i];
        }
        return lhs;
    }

    // These handle operations like "vector *= scalar" by implicitly converting a scalar to a vector.
    constexpr 
    VECTOR<T>& 
    operator*=(const VECTOR<T>& v) {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++) {
            lhs[i] *= v[i];
        }
        return lhs;
    }
    constexpr 
    VECTOR<T>& 
    operator/=(const VECTOR<T>& v) {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++) {
            lhs[i] /= v[i];
        }
        return lhs;
    }

    template<typename RT>
    friend inline constexpr NUM_PURE
    VECTOR<T>  
    operator*(VECTOR<T> lv, const VECTOR<RT>& rv) {
        return lv *= rv;
    }
    template<typename RT>
    friend inline constexpr NUM_PURE
    VECTOR<T>  
    operator/(VECTOR<T> lv, const VECTOR<RT>& rv) {
        return lv /= rv;
    }

    // These handle operations like "vector * scalar" and "scalar * vector" by implicitly converting a scalar to a vector.
    friend inline constexpr NUM_PURE
    VECTOR<T>  
    operator*(T lv, const VECTOR<T>& rv) {
        return VECTOR<T>(lv) *= rv;
    }
    friend inline constexpr NUM_PURE
    VECTOR<T>  
    operator*(VECTOR<T> lv, const VECTOR<T>& rv) {
        return lv *= rv;
    }
    friend inline constexpr NUM_PURE
    VECTOR<T>  
    operator/(VECTOR<T> lv, const VECTOR<T>& rv) {
        return lv /= rv;
    }
};


template<template<typename T> class VECTOR, typename T>
class VectorUnaryOperators {
public:
    VECTOR<T> 
    operator-() const {
        VECTOR<T> r;
        VECTOR<T> const& rv(static_cast<VECTOR<T> const&>(*this));
        for (size_t i = 0; i < r.size(); i++) {
            r[i] = -rv[i];
        }
        return r;
    }
};


template<template<typename T> class VECTOR, typename T>
class VectorComparisonOperators {
public:
    template<typename RT>
    friend inline NUM_PURE
    bool  
    operator==(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        // w/ inlining we end-up with many branches that will pollute the BPU cache
        NUM_NOUNROLL
        for (size_t i = 0; i < lv.size(); i++){
            if (!equivalent(lv[i], rv[i])) {
                return false;
            }
        }
        return true;
    }

    template<typename RT>
    friend inline NUM_PURE
    bool  
    operator!=(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        return !operator==(lv, rv);
    }

    template<typename RT>
    friend inline NUM_PURE
    bool  
    operator>(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        NUM_NOUNROLL
        for (size_t i = 0; i < lv.size(); i++) {
            if (lv[i] != rv[i]) {
                return lv[i] > rv[i];
            }
        }
        return false;
    }

    template<typename RT>
    friend inline constexpr NUM_PURE
    bool  
    operator<=(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        return !(lv > rv);
    }

    template<typename RT>
    friend inline NUM_PURE
    bool  
    operator<(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        NUM_NOUNROLL
            for (size_t i = 0; i < lv.size(); i++) {
                if (lv[i] != rv[i]) {
                    return lv[i] < rv[i];
                }
            }
        return false;
    }

    template<typename RT>
    friend inline constexpr NUM_PURE
    bool 
    operator>=(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        return !(lv < rv);
    }

    template<typename RT>
    friend inline NUM_PURE
    VECTOR<bool>  
    equal(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        VECTOR<bool> r;
        for (size_t i = 0; i < lv.size(); i++) {
            r[i] = lv[i] == rv[i];
        }
        return r;
    }

    template<typename RT>
    friend inline NUM_PURE
    VECTOR<bool>  
    not_equal(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        VECTOR<bool> r;
        for (size_t i = 0; i < lv.size(); i++) {
            r[i] = lv[i] != rv[i];
        }
        return r;
    }

    template<typename RT>
    friend inline NUM_PURE
    VECTOR<bool>  
    less_than(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        VECTOR<bool> r;
        for (size_t i = 0; i < lv.size(); i++) {
            r[i] = lv[i] < rv[i];
        }
        return r;
    }

    template<typename RT>
    friend inline NUM_PURE
    VECTOR<bool>  
    less_than_equal(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        VECTOR<bool> r;
        for (size_t i = 0; i < lv.size(); i++) {
            r[i] = lv[i] <= rv[i];
        }
        return r;
    }

    template<typename RT>
    friend inline NUM_PURE
    VECTOR<bool> 
    greater_than(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        VECTOR<bool> r;
        for (size_t i = 0; i < lv.size(); i++) {
            r[i] = lv[i] > rv[i];
        }
        return r;
    }

    template<typename RT>
    friend inline NUM_PURE
    VECTOR<bool>  
    greater_than_equal(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        VECTOR<bool> r;
        for (size_t i = 0; i < lv.size(); i++) {
            r[i] = lv[i] >= rv[i];
        }
        return r;
    }
};


template<template<typename T> class VECTOR, typename T>
class VectorFunctions {
public:
    template<typename RT>
    friend constexpr inline NUM_PURE
    T  
    dot(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        T r(0);
        for (size_t i = 0; i < lv.size(); i++) {
            r += lv[i] * rv[i];
        }
        return r;
    }

    friend inline NUM_PURE
    T  
    norm(const VECTOR<T>& lv) {
        return std::sqrt(dot(lv, lv));
    }

    friend inline NUM_PURE
    T  
    length(const VECTOR<T>& lv) {
        return norm(lv);
    }

    friend inline constexpr NUM_PURE
    T  
    norm2(const VECTOR<T>& lv) {
        return dot(lv, lv);
    }

    friend inline constexpr NUM_PURE
    T  
    length2(const VECTOR<T>& lv) {
        return norm2(lv);
    }

    template<typename RT>
    friend inline constexpr NUM_PURE
    T  
    distance(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        return length(rv - lv);
    }

    template<typename RT>
    friend inline constexpr NUM_PURE
    T  
    distance2(const VECTOR<T>& lv, const VECTOR<RT>& rv) {
        return length2(rv - lv);
    }

    friend inline constexpr NUM_PURE
    VECTOR<T>  
    normalize(const VECTOR<T>& lv) {
        return lv * (T(1) / length(lv));
    }

    friend inline NUM_PURE
    VECTOR<T>  
    rcp(VECTOR<T> v) {
        return T(1) / v;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    abs(VECTOR<T> v) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = std::abs(v[i]);
        }
        return v;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    floor(VECTOR<T> v) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = std::floor(v[i]);
        }
        return v;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    ceil(VECTOR<T> v) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = std::ceil(v[i]);
        }
        return v;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    round(VECTOR<T> v) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = std::round(v[i]);
        }
        return v;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    inversesqrt(VECTOR<T> v) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = T(1) / std::sqrt(v[i]);
        }
        return v;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    sqrt(VECTOR<T> v) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = std::sqrt(v[i]);
        }
        return v;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    pow(VECTOR<T> v, T p) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = std::pow(v[i], p);
        }
        return v;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    saturate(const VECTOR<T>& lv) {
        return clamp(lv, T(0), T(1));
    }

    friend inline NUM_PURE
    VECTOR<T>  
    clamp(VECTOR<T> v, T min, T max) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = std::min(max, std::max(min, v[i]));
        }
        return v;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    clamp(VECTOR<T> v, VECTOR<T> min, VECTOR<T> max) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = std::min(max[i], std::max(min[i], v[i]));
        }
        return v;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    fma(const VECTOR<T>& lv, const VECTOR<T>& rv, VECTOR<T> a) {
        for (size_t i = 0; i < lv.size(); i++) {
            //a[i] = std::fma(lv[i], rv[i], a[i]);
            a[i] += (lv[i] * rv[i]);
        }
        return a;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    min(const VECTOR<T>& u, VECTOR<T> v) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = std::min(u[i], v[i]);
        }
        return v;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    max(const VECTOR<T>& u, VECTOR<T> v) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = std::max(u[i], v[i]);
        }
        return v;
    }

    friend inline NUM_PURE
    T  
    max(const VECTOR<T>& v) {
        T r(std::numeric_limits<T>::lowest());
        for (size_t i = 0; i < v.size(); i++) {
            r = std::max(r, v[i]);
        }
        return r;
    }

    friend inline NUM_PURE
    T  
    min(const VECTOR<T>& v) {
        T r(std::numeric_limits<T>::max());
        for (size_t i = 0; i < v.size(); i++) {
            r = std::min(r, v[i]);
        }
        return r;
    }

    friend inline NUM_PURE
    VECTOR<T>  
    apply(VECTOR<T> v, const std::function<T(T)>& f) {
        for (size_t i = 0; i < v.size(); i++) {
            v[i] = f(v[i]);
        }
        return v;
    }

    friend inline NUM_PURE
    bool  
    any(const VECTOR<T>& v) {
        for (size_t i = 0; i < v.size(); i++) {
            if (v[i] != T(0)) return true;
        }
        return false;
    }

    friend inline NUM_PURE
    bool  
    all(const VECTOR<T>& v) {
        bool result = true;
        for (size_t i = 0; i < v.size(); i++) {
            result &= (v[i] != T(0));
        }
        return result;
    }

    template<typename R>
    friend inline NUM_PURE
    VECTOR<R>  
    map(VECTOR<T> v, const std::function<R(T)>& f) {
        VECTOR<R> result;
        for (size_t i = 0; i < v.size(); i++) {
            result[i] = f(v[i]);
        }
        return result;
    }
};


template<template<typename T> class VECTOR, typename T>
class VectorDebug {
public:
    friend 
    std::ostream& 
    operator<<(std::ostream& stream, const VECTOR<T>& v) {
        stream << "< ";
        for (size_t i = 0; i < v.size() - 1; i++) {
            stream << T(v[i]) << ", ";
        }
        stream << T(v[v.size() - 1]) << " >";
        return stream;
    }
};

} // namespace details
} // namespace numeric

#endif