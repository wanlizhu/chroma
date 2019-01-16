#ifndef CHROMA_IDEVICE_PROGRAM_INPUT_SLOT_H
#define CHROMA_IDEVICE_PROGRAM_INPUT_SLOT_H

#include <stdint.h>
#include <memory>
#include <string>
#include "idevice_resource.h"

namespace chroma {

class IDevice;

class IDeviceProgramInputSlot {
public:
    enum InputSlotType {
        ConstantBufferSlot, // aka. UniformBuffer
        ShaderIOBufferSlot,
        VertexAttributeSlot,
        TextureSlot,
        SamplerSlot
    };

    static IDeviceProgramInputSlot* create(InputSlotType type_, const std::string& name);
    static void destroy(IDeviceProgramInputSlot* instance);

    virtual bool is_valid() const noexcept = 0;
    virtual InputSlotType type() const noexcept = 0;
    virtual std::string name() const noexcept = 0;
    virtual bool connect(IDeviceResource* resource) noexcept = 0;
    virtual IDeviceResource* resource() noexcept = 0;
};

}

#endif