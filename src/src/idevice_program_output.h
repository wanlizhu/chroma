#ifndef CHROMA_IDEVICE_PROGRAM_OUTPUT_H
#define CHROMA_IDEVICE_PROGRAM_OUTPUT_H

#include <stdint.h>
#include <memory>
#include "idevice_program_output_target.h"
#include "idevice_program_output_swapchain.h"

namespace chroma {

class IDevice;

class IDeviceProgramOutput {
public:
    typedef IDeviceProgramOutputTarget Target;
    typedef IDeviceProgramOutputSwapChain SwapChain;

};

}

#endif