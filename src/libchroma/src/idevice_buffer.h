#ifndef CHROMA_IDEVICE_BUFFER_H
#define CHROMA_IDEVICE_BUFFER_H

#include <stdint.h>
#include "device_resource.h"
#include "ibuffer.h"

namespace chroma { namespace device {

class IDevice;

class IDeviceBuffer : public IBuffer, public DeviceResource {
public:
    enum Type {
        VertexBuffer,
        IndexBuffer,
        ShaderIOBuffer,
        ConstantBuffer
    };
    enum Map {
        Read,
        Write,
        ReadWrite,
        Coherent,
    };

    static IDeviceBuffer* create(Type type, IDevice* device, const void* data, size_t size_);
    static void destroy(IDeviceBuffer* instance);

    virtual bool is_coherent() const noexcept = 0;
    virtual bool is_mapped() const noexcept = 0;
    virtual char* map(Map option) noexcept = 0;
    virtual void unmap() noexcept = 0;
    virtual Type type() const noexcept = 0;
};

}} // namespace chroma -> device 

#endif