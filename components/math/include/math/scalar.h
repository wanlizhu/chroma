#ifndef CHROMA_MATH_SCALAR_H
#define CHROMA_MATH_SCALAR_H

#include <algorithm>
#include <cmath>
#include "details/compiler.h"

namespace math {

template<typename T>
inline constexpr T MATH_PURE saturate(T v) noexcept {
    return T(std::min(T(1), std::max(T(0), v)));
}

template<typename T>
inline constexpr T MATH_PURE clamp(T v, T min, T max) noexcept {
    return T(std::min(max, std::max(min, v)));
}

template<typename T>
inline constexpr T MATH_PURE mix(T x, T y, T a) noexcept {
    return x * (T(1) - a) + y * a;
}

template<typename T>
inline constexpr T MATH_PURE lerp(T x, T y, T a) noexcept {
    return mix(x, y, a);
}

} // namespace math

#endif