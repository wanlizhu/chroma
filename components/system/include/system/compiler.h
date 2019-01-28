#ifndef CHROMA_SYS_COMPILER_H
#define CHROMA_SYS_COMPILER_H

// compatibility with non-clang compilers...
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#ifndef __has_feature
#define __has_feature(x) 0
#endif

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if __has_attribute(visibility)
#    define SYS_PUBLIC  __attribute__((visibility("default")))
#else
#    define SYS_PUBLIC  
#endif

#if __has_attribute(visibility)
#    ifndef TNT_DEV
#        define SYS_PRIVATE __attribute__((visibility("hidden")))
#    else
#        define SYS_PRIVATE
#    endif
#else
#    define SYS_PRIVATE
#endif

/*
 * helps the compiler's optimizer predicting branches
 */
#if __has_builtin(__builtin_expect)
#   ifdef __cplusplus
#      define SYS_LIKELY(exp) (__builtin_expect( !!(exp), true ))
#      define SYS_UNLIKELY(exp) (__builtin_expect( !!(exp), false ))
#   else
#      define SYS_LIKELY(exp) (__builtin_expect( !!(exp), 1 ))
#      define SYS_UNLIKELY(exp) (__builtin_expect( !!(exp), 0 ))
#   endif
#else
#   define SYS_LIKELY(exp) (exp)
#   define SYS_UNLIKELY(exp) (exp)
#endif

#if __has_builtin(__builtin_prefetch)
#   define SYS_PREFETCH(exp) (__builtin_prefetch(exp))
#else
#   define SYS_PREFETCH(exp)
#endif

#if __has_builtin(__builtin_assume)
#   define SYS_ASSUME(exp) (__builtin_assume(exp))
#else
#   define SYS_ASSUME(exp)
#endif

#if (defined(__i386__) || defined(__x86_64__))
#   define SYS_HAS_HYPER_THREADING 1  // on x86 we assume we have hyper-threading.
#else
#   define SYS_HAS_HYPER_THREADING 0
#endif

#if defined(__EMSCRIPTEN__)
#   define SYS_HAS_THREADING 0
#else
#   define SYS_HAS_THREADING 1
#endif

#if __has_attribute(noinline)
#define SYS_NOINLINE __attribute__((noinline))
#else
#define SYS_NOINLINE
#endif

#if __has_attribute(always_inline)
#define SYS_ALWAYS_INLINE __attribute__((always_inline))
#else
#define SYS_ALWAYS_INLINE
#endif

#if __has_attribute(pure)
#define SYS_PURE __attribute__((pure))
#else
#define SYS_PURE
#endif

#if __has_attribute(maybe_unused)
#define SYS_UNUSED [[maybe_unused]]
#define SYS_UNUSED_IN_RELEASE [[maybe_unused]]
#elif __has_attribute(unused)
#define SYS_UNUSED __attribute__((unused))
#define SYS_UNUSED_IN_RELEASE __attribute__((unused))
#else
#define SYS_UNUSED
#define SYS_UNUSED_IN_RELEASE
#endif

#define SYS_RESTRICT __restrict__

#if __has_feature(cxx_thread_local)
#   ifdef ANDROID
#       // Android NDK lies about supporting cxx_thread_local
#       define SYS_HAS_FEATURE_CXX_THREAD_LOCAL 0
#   else // ANDROID
#       define SYS_HAS_FEATURE_CXX_THREAD_LOCAL 1
#   endif // ANDROID
#else
#   define SYS_HAS_FEATURE_CXX_THREAD_LOCAL 0
#endif

#if __has_feature(cxx_rtti)
#   define SYS_HAS_RTTI 1
#else
#   define SYS_HAS_RTTI 0
#endif

#ifdef __ARM_ACLE
#   include <arm_acle.h>
#   define SYS_WAIT_FOR_INTERRUPT()   __wfi()
#   define SYS_WAIT_FOR_EVENT()       __wfe()
#   define SYS_BROADCAST_EVENT()      __sev()
#   define SYS_SIGNAL_EVENT()         __sevl()
#   define SYS_PAUSE()                __yield()
#   define SYS_PREFETCHW(addr)        __pldx(1, 0, 0, addr)
#else // !__ARM_ACLE
#   if (defined(__i386__) || defined(__x86_64__))
#       define SYS_X86_PAUSE              {__asm__ __volatile__( "rep; nop" : : : "memory" );}
#       define SYS_WAIT_FOR_INTERRUPT()   SYS_X86_PAUSE
#       define SYS_WAIT_FOR_EVENT()       SYS_X86_PAUSE
#       define SYS_BROADCAST_EVENT()
#       define SYS_SIGNAL_EVENT()
#       define SYS_PAUSE()                SYS_X86_PAUSE
#       define SYS_PREFETCHW(addr)        SYS_PREFETCH(addr)
#   else // !x86
#       define SYS_WAIT_FOR_INTERRUPT()
#       define SYS_WAIT_FOR_EVENT()
#       define SYS_BROADCAST_EVENT()
#       define SYS_SIGNAL_EVENT()
#       define SYS_PAUSE()
#       define SYS_PREFETCHW(addr)        SYS_PREFETCH(addr)
#   endif // x86
#endif // __ARM_ACLE


// ssize_t is a POSIX type.
#if defined(WIN32)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#ifdef _MSC_VER
#   define SYS_EMPTY_BASES __declspec(empty_bases)
#else
#   define SYS_EMPTY_BASES
#endif

#if defined(WIN32)
    #define IMPORTSYMB __declspec(dllimport)
#else
    #define IMPORTSYMB
#endif

#endif 
