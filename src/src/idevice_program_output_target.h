#ifndef CHROMA_IDEVICE_PROGRAM_OUTPUT_TARGET_H
#define CHROMA_IDEVICE_PROGRAM_OUTPUT_TARGET_H

#include <stdint.h>
#include <memory>
#include "idevice_program_output_target_channel.h"

namespace chroma {

class IDevice;

// aka. Framebuffer
class IDeviceProgramOutputTarget {
public:
    typedef IDeviceProgramOutputTargetChannel Channel;
    enum Schema {
        RenderToTexture,
        RenderToBuffer,
    };

    static IDeviceProgramOutputTarget* create(Schema schema);
    static void destroy(IDeviceProgramOutputTarget* instance);

    virtual void set_channel(Channel* channel) noexcept = 0;

};

}

#endif