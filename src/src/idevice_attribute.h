#ifndef CHROMA_IDEVICE_ATTRIBUTE_H
#define CHROMA_IDEVICE_ATTRIBUTE_H

#include <stdint.h>
#include <math/vec2.h>

namespace chroma {

class IDeviceAttribute {
public:
    IDeviceAttribute() = default;
    virtual ~IDeviceAttribute() = default;

    virtual const char* vendor() const noexcept = 0;
    virtual const char* renderer() const noexcept = 0;
    virtual const char* version() const noexcept = 0;

    virtual uint32_t max_vertex_attribs_count() const noexcept = 0;
    virtual uint32_t max_vertex_count() const noexcept = 0;
    virtual uint32_t max_index_count() const noexcept = 0;
    virtual math::int2 max_texture_size() const noexcept = 0;
    virtual math::int2 max_frame_size() const noexcept = 0;
    virtual uint32_t max_depth() const noexcept = 0;
};

}

#endif