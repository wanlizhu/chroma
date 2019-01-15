#ifndef CHROMA_IDEVICE_TEXTURE_H
#define CHROMA_IDEVICE_TEXTURE_H

#include <stdint.h>
#include <memory>

namespace chroma {

class IDevice;

class IDeviceTexture : public std::enable_shared_from_this<IDeviceTexture> {
public:
    static std::shared_ptr<IDeviceTexture> create(std::shared_ptr<IDevice> device);

protected:
    IDeviceTexture() = default;
    IDeviceTexture(const IDeviceTexture&) = default;
    IDeviceTexture& operator=(const IDeviceTexture&) = default;
    virtual ~IDeviceTexture() = default;
};

}

#endif