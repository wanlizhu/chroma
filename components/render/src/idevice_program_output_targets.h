#ifndef CHROMA_IDEVICE_PROGRAM_OUTPUT_TARGET_H
#define CHROMA_IDEVICE_PROGRAM_OUTPUT_TARGET_H

#include <stdint.h>
#include <memory>
#include "idevice_texture.h"
#include "idevice_buffer.h"

namespace render { namespace device {

class IDevice;

class IDeviceProgramOutputTarget {
public:
    enum Type {
        ColorTarget,
        Aux1Target,
        Aux2Target,
        DepthTarget,
        StencilTarget
    };
    enum Storage {
        TextureStorage,
        BufferStorage,
    };

    virtual Storage storage() const noexcept = 0;
    virtual bool is_valid() const noexcept = 0;
    virtual IDeviceColorFormatID color_format() const noexcept = 0;
    virtual math::int2 size() const noexcept = 0;
};

class IDeviceProgramOutputTargetTexture 
    : public IDeviceProgramOutputTarget
    , public IDeviceTexture {
    typedef IDeviceProgramOutputTarget TargetBase;

    static IDeviceProgramOutputTargetTexture* create(IDevice* device, math::int2 size_, IDeviceColorFormatID format_);
    static void destroy(IDeviceProgramOutputTargetTexture* instance);

};

class IDeviceProgramOutputTargetBuffer 
    : public IDeviceProgramOutputTarget
    , public IDeviceBuffer {
    typedef IDeviceProgramOutputTarget TargetBase;

    static IDeviceProgramOutputTargetBuffer* create(IDevice* device, math::int2 size_, IDeviceColorFormatID format_);
    static void destroy(IDeviceProgramOutputTargetBuffer* instance);

};

}} // namespace render -> device

#endif