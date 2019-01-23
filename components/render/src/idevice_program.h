#ifndef CHROMA_IDEVICE_PROGRAM_H
#define CHROMA_IDEVICE_PROGRAM_H

#include <stdint.h>
#include <memory>
#include "idevice_program_input.h"
#include "idevice_program_output.h"
#include <utils/path.h>

namespace render { namespace device {

class IDevice;

class IDeviceProgram : public DeviceResource {
public:
    enum Shader {
        VertexShader,
        GeometryShader,
        PixelShader,
        ComputeShader
    };

    virtual std::string name() const noexcept = 0;
    virtual void shader_file(Shader shader, const utils::Path& path) noexcept = 0;
    virtual void shader_code(Shader shader, const std::string& path) noexcept = 0;
    virtual bool build() noexcept = 0;
    virtual std::string build_error() const noexcept = 0;
};

}} // namespace render -> device

#endif