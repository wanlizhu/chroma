#ifndef CHROMA_NUMERIC_COMPILER_H
#define CHROMA_NUMERIC_COMPILER_H

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin_expect)
#   ifdef __cplusplus
#      define NUM_LIKELY(exp)    (__builtin_expect(!!(exp), true))
#      define NUM_UNLIKELY(exp)  (__builtin_expect(!!(exp), false))
#   else
#      define NUM_LIKELY(exp)    (__builtin_expect(!!(exp), 1))
#      define NUM_UNLIKELY(exp)  (__builtin_expect(!!(exp), 0))
#   endif
#else
#   define NUM_LIKELY(exp)    (exp)
#   define NUM_UNLIKELY(exp)  (exp)
#endif

#if __has_attribute(pure)
#   define NUM_PURE __attribute__((pure))
#else
#   define NUM_PURE
#endif

#ifdef _MSC_VER
#   define NUM_EMPTY_BASES __declspec(empty_bases)
// MSVC does not support loop unrolling hints
#   define NUM_NOUNROLL

// Sadly, MSVC does not support __builtin_constant_p
#   ifndef MAKE_CONSTEXPR
#       define MAKE_CONSTEXPR(e) (e)
#   endif

#else // _MSC_VER

#   define NUM_EMPTY_BASES
// C++11 allows pragmas to be specified as part of defines using the _Pragma syntax.
#   define NUM_NOUNROLL _Pragma("nounroll")

#   ifndef MAKE_CONSTEXPR
#       define MAKE_CONSTEXPR(e) __builtin_constant_p(e) ? (e) : (e)
#   endif
#endif // _MSC_VER

#endif