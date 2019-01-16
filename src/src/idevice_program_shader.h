#ifndef CHROMA_IDEVICE_PROGRAM_SHADER_H
#define CHROMA_IDEVICE_PROGRAM_SHADER_H

#include <stdint.h>
#include "idevice_resource.h"
#include <utils/path.h>

namespace chroma {

class IDeviceProgramShader : public IDeviceResource {
public:
    enum ShaderType {
        VertexShader,
        GeometryShader,
        TessEvalShader,
        TessControlShader,
        FragmentShader,
        ComputeShader
    };

    static IDeviceProgramShader* create(ShaderType type_);
    static void destroy(IDeviceProgramShader* shader);

    virtual bool is_valid() const noexcept = 0;
    virtual ShaderType type() const noexcept = 0;
    virtual bool compile(const utils::Path& path) noexcept = 0;
    virtual bool compile(const char* src) noexcept = 0;
    virtual std::string compile_error() const noexcept = 0;
};

}

#endif