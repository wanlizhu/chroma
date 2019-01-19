#ifndef CHROMA_IDEVICE_PROGRAM_INPUT_SLOT_H
#define CHROMA_IDEVICE_PROGRAM_INPUT_SLOT_H

#include <stdint.h>
#include <memory>
#include <string>
#include "idevice_resource.h"

namespace chroma { namespace device {

class IDevice;

class IDeviceProgramInputSlot {
public:
    enum Type {
        UndefinedSlot,
        ConstantBufferSlot, // aka. UniformBuffer
        ShaderIOBufferSlot,
        VertexAttributeSlot,
        TextureSlot,
        SamplerSlot
    };

    static IDeviceProgramInputSlot* create(IDevice* device, Type type_, const std::string& name);
    static void destroy(IDeviceProgramInputSlot* instance);

    virtual bool is_valid() const noexcept = 0;
    virtual Type type() const noexcept = 0;
    virtual std::string name() const noexcept = 0;
    virtual bool connect(IDeviceResource* resource) noexcept = 0;
    virtual IDeviceResource* resource() noexcept = 0;
};

}} // namespace chroma -> device

#endif