#ifndef CHROMA_IDEVICE_BUFFER_H
#define CHROMA_IDEVICE_BUFFER_H

#include <stdint.h>
#include "idevice_resource.h"
#include "ibuffer.h"

namespace chroma {

class IDevice;

class IDeviceBuffer : public IBuffer, public IDeviceResource {
public:
    enum Type {
        STATIC_DRAW,
        DYNAMIC_DRAW,
        STORAGE,
    };
    enum MapOption {
        READ,
        WRITE,
        READ_WRITE,
        COHERENT,
    };

    static IDeviceBuffer* create(const void* data, size_t size_, Type type);
    static void destroy(IDeviceBuffer* instance);

    virtual bool is_coherent() const noexcept = 0;
    virtual bool is_mapped() const noexcept = 0;
    virtual char* map(MapOption option) noexcept = 0;
    virtual void unmap() noexcept = 0;

protected:
    IDeviceBuffer() = default;
    IDeviceBuffer(const IDeviceBuffer&) = default;
    IDeviceBuffer& operator=(const IDeviceBuffer&) = default;
    virtual ~IDeviceBuffer() = default;
};

}

#endif