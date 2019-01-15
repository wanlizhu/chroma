#ifndef CHROMA_IDEVICE_BUFFER_H
#define CHROMA_IDEVICE_BUFFER_H

#include <stdint.h>
#include "idevice_resource.h"

namespace chroma {

class IDevice;

class IDeviceBuffer : public IDeviceResource {
public:
    enum Type {
        STATIC,
        DYNAMIC,
        COHERENT,
    };

    virtual size_t size() const noexcept = 0;
    virtual bool is_coherent() const noexcept = 0;
    virtual bool read(size_t offset, size_t size_, void* dst) const noexcept = 0;
    virtual bool write(size_t offset, size_t size_, const void* src) noexcept = 0;

protected:
    IDeviceBuffer() = default;
    IDeviceBuffer(const IDeviceBuffer&) = default;
    IDeviceBuffer& operator=(const IDeviceBuffer&) = default;
    virtual ~IDeviceBuffer() = default;
};

}

#endif