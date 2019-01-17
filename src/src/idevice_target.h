#ifndef CHROMA_IDEVICE_TARGET_H
#define CHROMA_IDEVICE_TARGET_H

#include <stdint.h>
#include <memory>
#include "idevice_resource.h"

namespace chroma { namespace device {

class IDevice;

class IDeviceTarget : public IDeviceResource {
public:
    static IDeviceTarget* create();

};

}} // namespace chroma -> device

#endif