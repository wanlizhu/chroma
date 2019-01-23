#ifndef CHROMA_NUMERIC_FAST_H
#define CHROMA_NUMERIC_FAST_H

#include <cmath>
#include <stdint.h>
#include <type_traits>
#include "details/compiler.h"

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

namespace numeric {
namespace fast {

// fast cos(x), ~8 cycles (vs. 66 cycles on ARM)
// can be vectorized
// x between -pi and pi
template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
constexpr NUM_PURE
T  
cos(T x) noexcept {
    x *= T(M_1_PI / 2);
    x -= T(0.25) + std::floor(x + T(0.25));
    x *= T(16.0) * std::abs(x) - T(8.0);
    x += T(0.225) * x * (std::abs(x) - T(1.0));
    return x;
}

// fast sin(x), ~8 cycles (vs. 66 cycles on ARM)
// can be vectorized
// x between -pi and pi
template <typename T, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
constexpr NUM_PURE
T
sin(T x) noexcept {
    return numeric::fast::cos<T>(x - T(M_PI_2));
}

constexpr inline NUM_PURE
float
ilog2(float x) noexcept {
    union {
        float val;
        int32_t x;
    } u = { x };
    return ((u.x >> 23) & 0xff) - 127;
}

constexpr inline NUM_PURE
float
log2(float x) noexcept {
    union {
        float val;
        int32_t x;
    } u = { x };
    float ilog2 = float(((u.x >> 23) & 0xff) - 128);
    u.x = (u.x & 0x007fffff) | 0x3f800000;
    return ilog2 + (-0.34484843f * u.val + 2.02466578f) * u.val - 0.67487759f;
}

// fast 1/sqrt(), on ARMv8 this is 5 cycles vs. 7 cycles, so maybe not worth it.
// we keep this mostly for reference and benchmarking.
inline NUM_PURE
float
isqrt(float x) noexcept {
#if defined(__ARM_NEON) && defined(__aarch64__)
    float y = vrsqrtes_f32(x);
    return y * vrsqrtss_f32(x, y * y);
#else
    return 1 / std::sqrt(x);
#endif
}

inline NUM_PURE
double  
isqrt(double x) noexcept {
#if defined(__ARM_NEON) && defined(__aarch64__)
    double y = vrsqrted_f64(x);
    return y * vrsqrtsd_f64(x, y * y);
#else
    return 1 / std::sqrt(x);
#endif
}

inline 
int
signbit(float x) noexcept {
#if __has_builtin(__builtin_signbitf)
    // Note: on Android NDK, signbit() is a function call -- not what we want.
    return __builtin_signbitf(x);
#else
    return std::signbit(x);
#endif
}

constexpr 
double
pow(double x, unsigned int y) noexcept {
    return y == 0 ? 1.0 : x * pow(x, y - 1);
}

constexpr
unsigned int
factorial(unsigned int x) noexcept {
    return x == 0 ? 1 : x * factorial(x - 1);
}

constexpr
double
exp(double x) noexcept {
    return 1.0 + x + pow(x, 2) / factorial(2) + pow(x, 3) / factorial(3)
        + pow(x, 4) / factorial(4) + pow(x, 5) / factorial(5)
        + pow(x, 6) / factorial(6) + pow(x, 7) / factorial(7)
        + pow(x, 8) / factorial(8) + pow(x, 9) / factorial(9);
}

constexpr 
float
exp(float x) noexcept {
    return float(exp(double(x)));
}

inline NUM_PURE
float 
pow(float a, float b) noexcept {
    constexpr int fudgeMinRMSE = 486411;
    constexpr int K = (127 << 23) - fudgeMinRMSE;
    union { float f; int x; } u = { a };
    u.x = (int)(b * (u.x - K) + K);
    return u.f;
}

// this is more precise than pow() above
inline NUM_PURE
float 
pow2dot2(float a) noexcept {
    union { float f; int x; } u = { a };
    constexpr int K = (127 << 23);
    u.x = (int)(0.2f * (u.x - K) + K);
    return a * a * u.f; // a^2 * a^0.2
}

// unsigned saturated arithmetic
#if defined(__ARM_NEON) && defined(__aarch64__)
inline NUM_PURE uint8_t  qadd(uint8_t a, uint8_t b)  noexcept { return vuqaddb_s8(a, b); }
inline NUM_PURE uint16_t qadd(uint16_t a, uint16_t b) noexcept { return vuqaddh_s16(a, b); }
inline NUM_PURE uint32_t qadd(uint32_t a, uint32_t b) noexcept { return vuqadds_s32(a, b); }

inline NUM_PURE uint8_t  qsub(uint8_t a, uint8_t b)  noexcept { return vqsubb_s8(a, b); }
inline NUM_PURE uint16_t qsub(uint16_t a, uint16_t b) noexcept { return vqsubh_s16(a, b); }
inline NUM_PURE uint32_t qsub(uint32_t a, uint32_t b) noexcept { return vqsubs_s32(a, b); }
#else
template<typename T, typename = typename std::enable_if<
    std::is_same<uint8_t, T>::value ||
    std::is_same<uint16_t, T>::value ||
    std::is_same<uint32_t, T>::value>::type>
inline NUM_PURE
T
qadd(T a, T b)  noexcept {
    T r = a + b;
    return r | -T(r < a);
}

template<typename T, typename = typename std::enable_if<
    std::is_same<uint8_t, T>::value ||
    std::is_same<uint16_t, T>::value ||
    std::is_same<uint32_t, T>::value>::type>
inline NUM_PURE
T
qsub(T a, T b)  noexcept {
    T r = a - b;
    return r & -T(r <= a);
}
#endif

template<typename T>
inline NUM_PURE
T
qinc(T a)  noexcept {
    return qadd(a, T(1));
}

template<typename T>
inline NUM_PURE
T
qdec(T a)  noexcept {
    return qsub(a, T(1));
}

} // namespace fast
} // namespace numeric

#endif