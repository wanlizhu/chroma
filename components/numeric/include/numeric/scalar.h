#ifndef CHROMA_NUMERIC_SCALAR_H
#define CHROMA_NUMERIC_SCALAR_H

#include <algorithm>
#include <cmath>
#include "details/compiler.h"

namespace numeric {

template<typename T>
inline constexpr NUM_PURE
T  
saturate(T v) noexcept {
    return T(std::min(T(1), std::max(T(0), v)));
}

template<typename T>
inline constexpr NUM_PURE
T  
clamp(T v, T min, T max) noexcept {
    return T(std::min(max, std::max(min, v)));
}

template<typename T>
inline constexpr NUM_PURE
T  
mix(T x, T y, T a) noexcept {
    return x * (T(1) - a) + y * a;
}

template<typename T>
inline constexpr NUM_PURE
T 
lerp(T x, T y, T a) noexcept {
    return mix(x, y, a);
}

template<typename T>
inline constexpr NUM_PURE
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
equivalent(const T& lhs, const T& rhs) noexcept {
    return std::abs(lhs - rhs) <= std::numeric_limits<T>::epsilon();
}

template<typename T>
inline constexpr NUM_PURE
typename std::enable_if<!std::is_floating_point<T>::value, bool>::type
equivalent(const T& lhs, const T& rhs) noexcept {
    return lhs == rhs;
}

} // namespace numeric

#endif