#ifndef CHROMA_IDEVICE_RESOURCE_H
#define CHROMA_IDEVICE_RESOURCE_H

#include <stdint.h>

namespace chroma {

class IDeviceResource {
public:
    enum ResourceType {
        Undefined,
        VertexBuffer,
        IndexBuffer,
        RenderBuffer,
        ShaderIOBuffer,
        ConstantBuffer,
        Texture2D,
        TextureCubeMap,
        SamplerObject
    };

    inline int id() const noexcept { return m_id; }
    inline ResourceType type() const noexcept { return m_type; }
    inline bool is_valid() const noexcept { return m_id != -1; }

protected:
    IDeviceResource(ResourceType type_, int id_)
        : m_type(type_), m_id(id_) {}
    IDeviceResource(const IDeviceResource&) = default;
    IDeviceResource& operator=(const IDeviceResource&) = default;
    virtual ~IDeviceResource() = default;

    inline void set_id(int id_) noexcept { m_id = id_; }

private:
    ResourceType m_type = ResourceType::Undefined;
    int m_id = -1;
};

}

#endif