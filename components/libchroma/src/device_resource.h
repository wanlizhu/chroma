#ifndef CHROMA_DEVICE_RESOURCE_H
#define CHROMA_DEVICE_RESOURCE_H

#include <stdint.h>
#include <memory>

namespace chroma { namespace device {

class DeviceResource {
public:
    enum Type {
        UndefinedResource,
        BufferResource,
        RenderBufferResource,
        TextureResource,
        SamplerResource
    };

    inline int id() const noexcept { return m_id; }
    inline Type type() const noexcept { return m_type; }
    inline bool is_valid() const noexcept { return m_id != -1; }

protected:
    DeviceResource(Type type_, int id_)
        : m_type(type_), m_id(id_) {}
    DeviceResource(const DeviceResource&) = default;
    DeviceResource& operator=(const DeviceResource&) = default;
    virtual ~DeviceResource() = default;

    inline void set_id(int id_) noexcept { m_id = id_; }

private:
    Type m_type = Type::UndefinedResource;
    int m_id = -1;
};

}} // namespace chroma -> device

#endif