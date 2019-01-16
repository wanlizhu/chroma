#ifndef CHROMA_IDEVICE_TARGET_H
#define CHROMA_IDEVICE_TARGET_H

#include <stdint.h>
#include <memory>

namespace chroma {

class IDevice;

class IDeviceTarget : public std::enable_shared_from_this<IDeviceTarget> {
public:
    static std::shared_ptr<IDeviceTarget> create(std::shared_ptr<IDevice> device);

};

}

#endif