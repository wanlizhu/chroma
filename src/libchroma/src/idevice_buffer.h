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

    virtual size_t size() const noexcept = 0;
    virtual IDeviceBuffer::Type type() const noexcept = 0;

protected:
    IDeviceBuffer() = default;
    virtual ~IDeviceBuffer() = default;
};

}} // namespace chroma -> device 

#endif