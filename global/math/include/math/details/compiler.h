#ifndef CHROMA_MATH_COMPILER_H
#define CHROMA_MATH_COMPILER_H

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin_expect)
#   ifdef __cplusplus
#      define MATH_LIKELY(exp)    (__builtin_expect(!!(exp), true))
#      define MATH_UNLIKELY(exp)  (__builtin_expect(!!(exp), false))
#   else
#      define MATH_LIKELY(exp)    (__builtin_expect(!!(exp), 1))
#      define MATH_UNLIKELY(exp)  (__builtin_expect(!!(exp), 0))
#   endif
#else
#   define MATH_LIKELY(exp)    (exp)
#   define MATH_UNLIKELY(exp)  (exp)
#endif

#if __has_attribute(pure)
#   define MATH_PURE __attribute__((pure))
#else
#   define MATH_PURE
#endif

#ifdef _MSC_VER
#   define MATH_EMPTY_BASES __declspec(empty_bases)
// MSVC does not support loop unrolling hints
#   define MATH_NOUNROLL

// Sadly, MSVC does not support __builtin_constant_p
#   ifndef MAKE_CONSTEXPR
#       define MAKE_CONSTEXPR(e) (e)
#   endif

#else // _MSC_VER

#   define MATH_EMPTY_BASES
// C++11 allows pragmas to be specified as part of defines using the _Pragma syntax.
#   define MATH_NOUNROLL _Pragma("nounroll")

#   ifndef MAKE_CONSTEXPR
#       define MAKE_CONSTEXPR(e) __builtin_constant_p(e) ? (e) : (e)
#   endif
#endif // _MSC_VER

namespace math {

struct GPUDeviceCompatible {};

}
#endif