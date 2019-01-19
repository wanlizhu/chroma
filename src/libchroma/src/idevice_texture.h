#ifndef CHROMA_IDEVICE_TEXTURE_H
#define CHROMA_IDEVICE_TEXTURE_H

#include <stdint.h>
#include <memory>
#include "idevice_texture_sampler.h"

namespace chroma { namespace device {

class IDevice;

class IDeviceTexture : public IDeviceResource {
public:
    static IDeviceTexture* create(IDevice* device, math::int2 size_, IDeviceColorFormatID format_);
    static void destroy(IDeviceTexture* instance);
};

}} // namespace chroma -> device

#endif