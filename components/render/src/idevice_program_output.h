#ifndef CHROMA_IDEVICE_PROGRAM_OUTPUT_H
#define CHROMA_IDEVICE_PROGRAM_OUTPUT_H

#include <stdint.h>
#include <memory>
#include "idevice_program_output_targets.h"
#include "idevice_program_output_swapchain.h"

namespace render { namespace device {

class IDevice;

// aka. Framebuffer
class IDeviceProgramOutput {
public:
    typedef IDeviceProgramOutputTarget Target;
    typedef IDeviceProgramOutputSwapChain SwapChain;
    enum Schema {
        Undefined,
        RenderToTexture,
        RenderToBuffer,
    };

    static IDeviceProgramOutput* create(IDevice* device, Schema schema);
    static void destroy(IDeviceProgramOutput* instance);

    virtual void connect(Target* target) noexcept = 0;
    virtual void disconnect(Target::Type type) noexcept = 0;
    virtual Schema schema() const noexcept = 0;
    virtual Target* target(Target::Type type) noexcept = 0;
};

}} // namespace render -> device

#endif