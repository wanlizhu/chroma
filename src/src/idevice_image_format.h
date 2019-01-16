#ifndef CHROMA_IDEVICE_IMAGE_FORMAT_H
#define CHROMA_IDEVICE_IMAGE_FORMAT_H

#include <stdint.h>
#include "idevice_resource.h"

namespace chroma {

class IDevice;

enum IDeviceImageFormat {
    SRGB,
    SRGBA,

    RGB_8i,
    RGBA_8i,
    RGB_32f,
    RGBA_32f,

    DEPTH_16i,
    DEPTH_24i,
    DEPTH_32f,

    STENCIL_8i,
    DEPTH24_AND_STENCIL8,
    DEPTH32F_AND_STENCIL8,
};

}

#endif