#ifndef CHROMA_IDEVICE_PROGRAM_H
#define CHROMA_IDEVICE_PROGRAM_H

#include <stdint.h>
#include <memory>
#include "idevice_program_input.h"
#include "idevice_program_output.h"
#include <utils/path.h>

namespace chroma { namespace device {

class IDevice;

template<typename INPUT, typename OUTPUT>
class IDeviceProgram : public DeviceResource {
public:
    typedef INPUT Input;
    typedef OUTPUT Output;
    typedef INPUT::Slot Slot;
    typedef OUTPUT::Target Target;
    typedef OUTPUT::SwapChain SwapChain;

    static IDeviceProgram* create(IDevice* device, Input::Schema schema, const std::string& name_);
    static void destroy(IDeviceProgram* instance);

    virtual std::string name() const noexcept = 0;
    virtual void shader_file(const utils::Path& path) noexcept = 0;
    virtual void shader_code(const std::string& path) noexcept = 0;
    virtual bool build() noexcept = 0;
    virtual std::string build_error() const noexcept = 0;
    virtual Input* input() noexcept = 0;
    virtual Output* output() noexcept = 0;
};

}} // namespace chroma -> device

#endif