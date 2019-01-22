#ifndef CHROMA_MATH_NORM_H
#define CHROMA_MATH_NORM_H

#include "scalar.h"
#include "vec4.h"

#include <cmath>
#include <stdint.h>

namespace math {

inline uint16_t pack_unorm16(float v) noexcept {
    return static_cast<uint16_t>(std::round(clamp(v, 0.0f, 1.0f) * 65535.0f));
}

inline ushort4 pack_unorm16(float4 v) noexcept {
    return ushort4{ pack_unorm16(v.x), pack_unorm16(v.y), pack_unorm16(v.z), pack_unorm16(v.w) };
}

inline int16_t pack_snorm16(float v) noexcept {
    return static_cast<int16_t>(std::round(clamp(v, -1.0f, 1.0f) * 32767.0f));
}

inline short2 pack_snorm16(float2 v) noexcept {
    return short2{ pack_snorm16(v.x), pack_snorm16(v.y) };
}

inline short4 pack_snorm16(float4 v) noexcept {
    return short4{ pack_snorm16(v.x), pack_snorm16(v.y), pack_snorm16(v.z), pack_snorm16(v.w) };
}

inline float unpack_unorm16(uint16_t v) noexcept {
    return v / 65535.0f;
}

inline float4 unpack_unorm16(ushort4 v) noexcept {
    return float4{ unpack_unorm16(v.x), unpack_unorm16(v.y), unpack_unorm16(v.z), unpack_unorm16(v.w) };
}

inline float unpack_snorm16(int16_t v) noexcept {
    return clamp(v / 32767.0f, -1.0f, 1.0f);
}

inline float4 unpack_snorm16(short4 v) noexcept {
    return float4{ unpack_snorm16(v.x), unpack_snorm16(v.y), unpack_snorm16(v.z), unpack_snorm16(v.w) };
}

inline uint8_t pack_unorm8(float v) noexcept {
    return static_cast<uint8_t>(std::round(clamp(v, 0.0f, 1.0f) * 255.0));
}

inline ubyte4 pack_unorm8(float4 v) noexcept {
    return ubyte4{ pack_unorm8(v.x), pack_unorm8(v.y), pack_unorm8(v.z), pack_unorm8(v.w) };
}

inline int8_t pack_snorm8(float v) noexcept {
    return static_cast<int8_t>(std::round(clamp(v, -1.0f, 1.0f) * 127.0));
}

inline byte4 pack_snorm8(float4 v) noexcept {
    return byte4{ pack_snorm8(v.x), pack_snorm8(v.y), pack_snorm8(v.z), pack_snorm8(v.w) };
}

inline float unpack_unorm8(uint8_t v) noexcept {
    return v / 255.0f;
}

inline float4 unpack_unorm8(ubyte4 v) noexcept {
    return float4{ unpack_unorm8(v.x), unpack_unorm8(v.y), unpack_unorm8(v.z), unpack_unorm8(v.w) };
}

inline float unpack_snorm8(int8_t v) noexcept {
    return clamp(v / 127.0f, -1.0f, 1.0f);
}

inline float4 unpack_snorm8(byte4 v) noexcept {
    return float4{ unpack_snorm8(v.x), unpack_snorm8(v.y), unpack_snorm8(v.z), unpack_snorm8(v.w) };
}

} // namespace std

#endif