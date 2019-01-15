#ifndef CHROMA_IDEVICE_PROGRAM_H
#define CHROMA_IDEVICE_PROGRAM_H

#include <stdint.h>
#include <memory>
#include "idevice_program_input.h"
#include "idevice_program_output.h"

namespace chroma {

class IDevice;

class IDeviceProgram : public IDeviceResource {
public:
    static IDeviceProgram* create();
    static void destroy(IDeviceProgram* instance);

    virtual IDeviceProgramInput* input() noexcept = 0;
    virtual IDeviceProgramOutput* output() noexcept = 0;

protected:
    IDeviceProgram() = default;
    IDeviceProgram(const IDeviceProgram&) = default;
    IDeviceProgram& operator=(const IDeviceProgram&) = default;
    virtual ~IDeviceProgram() = default;
};

}

#endif