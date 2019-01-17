#ifndef CHROMA_IDEVICE_PROGRAM_OUTPUT_TARGET_H
#define CHROMA_IDEVICE_PROGRAM_OUTPUT_TARGET_H

#include <stdint.h>
#include <memory>
#include "idevice_texture.h"
#include "idevice_buffer.h"

namespace chroma { namespace device {

class IDevice;

class IDeviceProgramOutputTarget {
public:
    enum StorageType {
        TextureStorage,
        BufferStorage,
    };

    virtual StorageType storage() const noexcept = 0;
    virtual bool is_valid() const noexcept = 0;
    virtual IDeviceColorFormatID color_format() const noexcept = 0;
    virtual math::int2 size() const noexcept = 0;
};

class IDeviceProgramOutputTargetTexture 
    : public IDeviceProgramOutputTarget
    , public IDeviceTexture {
    static IDeviceProgramOutputTargetTexture* create(math::int2 size_, IDeviceColorFormatID format_);
    static void destroy(IDeviceProgramOutputTargetTexture* instance);

    virtual StorageType storage() const noexcept override { return StorageType::TextureStorage; }
};

class IDeviceProgramOutputTargetBuffer 
    : public IDeviceProgramOutputTarget
    , public IDeviceBuffer {
    static IDeviceProgramOutputTargetBuffer* create(math::int2 size_, IDeviceColorFormatID format_);
    static void destroy(IDeviceProgramOutputTargetBuffer* instance);

    virtual StorageType storage() const noexcept override { return StorageType::BufferStorage; }
};

}} // namespace chroma -> device

#endif