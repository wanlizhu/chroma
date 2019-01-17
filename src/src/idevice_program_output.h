#ifndef CHROMA_IDEVICE_PROGRAM_OUTPUT_H
#define CHROMA_IDEVICE_PROGRAM_OUTPUT_H

#include <stdint.h>
#include <memory>
#include "idevice_program_output_targets.h"
#include "idevice_program_output_swapchain.h"

namespace chroma { namespace device {

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
    enum TargetType {
        Color,
        Aux1,
        Aux2,
        Depth,
        Stencil
    };

    static IDeviceProgramOutput* create(Schema schema);
    static void destroy(IDeviceProgramOutput* instance);

    virtual void connect(TargetType type, Target* target) noexcept = 0;
    virtual void disconnect(TargetType type) noexcept = 0;
    virtual Schema schema() const noexcept = 0;
    virtual Target* target(TargetType type) noexcept = 0;
};

}} // namespace chroma -> device

#endif