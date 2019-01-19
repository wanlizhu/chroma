#ifndef CHROMA_IDEVICE_TEXTURE_SAMPLER_H
#define CHROMA_IDEVICE_TEXTURE_SAMPLER_H

#include <stdint.h>
#include <memory>
#include "idevice_resource.h"
#include "idevice_color_format.h"

namespace chroma { namespace device {

class IDevice;

class IDeviceTextureSampler : public IDeviceResource {
public:
    static IDeviceTextureSampler* create(IDevice* device);
    static void destroy(IDeviceTextureSampler* instance);

};

}} // namespace chroma -> device

#endif