#ifndef CHROMA_IDEVICE_BASIC_TYPES_H
#define CHROMA_IDEVICE_BASIC_TYPES_H

#include <stdint.h>
#include <math/half.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>
#include <math/mat2.h>
#include <math/mat3.h>
#include <math/mat4.h>
#include <math/quat.h>

namespace render { namespace device {

typedef int8_t INT8;
typedef int16_t INT16;
typedef int32_t INT32;
typedef int64_t INT64;

typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;

typedef math::half FLOAT16;
typedef float FLOAT32;
typedef double FLOAT64;

template<typename T>
struct is_device_scalar {
    typedef T type;
    static constexpr const bool value = (std::is_integral<T>::value
        || std::is_floating_point<T>::value
        || std::is_same<math::half, T>::value);
};

template<typename T>
struct is_device_vector {
    typedef T type;
    static constexpr const bool value = 
        (!std::is_same<math::half, T>::value
        && (std::is_same<math::float2, T>::value
            || std::is_same<math::float3, T>::value
            || std::is_same<math::float4, T>::value
            || std::is_same<math::quatf, T>::value
            || std::is_same<math::mat2f, T>::value
            || std::is_same<math::mat3f, T>::value
            || std::is_same<math::mat4f, T>::value
            || std::is_same<math::double2, T>::value
            || std::is_same<math::double3, T>::value
            || std::is_same<math::double4, T>::value
            || std::is_same<math::quat, T>::value
            || std::is_same<math::mat2, T>::value
            || std::is_same<math::mat3, T>::value
            || std::is_same<math::mat4, T>::value
           )
        );
};

template<typename T>
struct is_device_type {
    typedef T type;
    static constexpr const bool value = (is_device_scalar<T>::value || is_device_vector<T>::value);
};



}} // namespace render -> device

#endif