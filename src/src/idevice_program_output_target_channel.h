#ifndef CHROMA_IDEVICE_PROGRAM_OUTPUT_TARGET_CHANNEL_H
#define CHROMA_IDEVICE_PROGRAM_OUTPUT_TARGET_CHANNEL_H

#include <stdint.h>
#include <memory>
#include <math/vec2.h>
#include "idevice_resource.h"
#include "idevice_image_format.h"

namespace chroma {

class IDevice;

// aka. Attachment of Framebuffer
class IDeviceProgramOutputTargetChannel : public IDeviceResource {
public:
    typedef IDeviceImageFormat Format;
    enum ChannelType {
        Color,
        AuxColor1,
        AuxColor2,
        Depth,
        Stencil
    };
    enum StorageType {
        TextureStorage,
        RenderBufferStorage
    };

    static IDeviceProgramOutputTargetChannel* create(Format format_, math::int2 size_, ChannelType type_, StorageType storage_);
    static void destroy(IDeviceProgramOutputTargetChannel* instance);

    virtual bool is_valid() const noexcept = 0;
    virtual Format format() const noexcept = 0;
    virtual math::int2 size() const noexcept = 0;
    virtual ChannelType type() const noexcept = 0;
    virtual StorageType storage() const noexcept = 0;
};

}

#endif