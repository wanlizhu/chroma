#ifndef CHROMA_RENDER_DEVICE_TYPES_H
#define CHROMA_RENDER_DEVICE_TYPES_H

#include <stdint.h>
#include <numeric/half.h>
#include <numeric/vec2.h>
#include <numeric/vec3.h>
#include <numeric/vec4.h>
#include <numeric/mat2.h>
#include <numeric/mat3.h>
#include <numeric/mat4.h>
#include <numeric/quat.h>

namespace render { 

typedef int8_t INT8;
typedef int16_t INT16;
typedef int32_t INT32;
typedef int64_t INT64;

typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;

typedef numeric::half FLOAT16;
typedef float FLOAT32;
typedef double FLOAT64;

template<typename T>
struct is_device_scalar {
    typedef T type;
    static constexpr const bool value = (std::is_integral<T>::value
        || std::is_floating_point<T>::value
        || std::is_same<numeric::half, T>::value);
};

template<typename T>
struct is_device_vector {
    typedef T type;
    static constexpr const bool value = 
        (!std::is_same<numeric::half, T>::value
        && (std::is_same<numeric::float2, T>::value
            || std::is_same<numeric::float3, T>::value
            || std::is_same<numeric::float4, T>::value
            || std::is_same<numeric::quatf, T>::value
            || std::is_same<numeric::mat2f, T>::value
            || std::is_same<numeric::mat3f, T>::value
            || std::is_same<numeric::mat4f, T>::value
            || std::is_same<numeric::double2, T>::value
            || std::is_same<numeric::double3, T>::value
            || std::is_same<numeric::double4, T>::value
            || std::is_same<numeric::quat, T>::value
            || std::is_same<numeric::mat2, T>::value
            || std::is_same<numeric::mat3, T>::value
            || std::is_same<numeric::mat4, T>::value
           )
        );
};

template<typename T>
struct is_device_type {
    typedef T type;
    static constexpr const bool value = (is_device_scalar<T>::value || is_device_vector<T>::value);
};

} // namespace render

#endif