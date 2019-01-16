#ifndef CHROMA_IDEVICE_PROGRAM_H
#define CHROMA_IDEVICE_PROGRAM_H

#include <stdint.h>
#include <memory>
#include "idevice_program_input.h"
#include "idevice_program_output.h"
#include "idevice_program_shader.h"

namespace chroma {

class IDevice;

class IDeviceProgram : public IDeviceResource {
public:
    typedef IDeviceProgramInput Input;
    typedef IDeviceProgramOutput Output;
    typedef IDeviceProgramShader Shader;
    typedef Input::Slot Slot;
    typedef Output::Target Target;
    typedef Output::SwapChain SwapChain;

    static IDeviceProgram* create(Input::Schema schema);
    static void destroy(IDeviceProgram* instance);

    virtual void set_shader(Shader* shader) noexcept = 0;
    virtual bool build() noexcept = 0;
    virtual std::string build_error() const noexcept = 0;
    virtual Input* input() noexcept = 0;
    virtual Output* output() noexcept = 0;
};

}

#endif