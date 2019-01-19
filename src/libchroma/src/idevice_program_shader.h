#ifndef CHROMA_IDEVICE_PROGRAM_SHADER_H
#define CHROMA_IDEVICE_PROGRAM_SHADER_H

#include <stdint.h>
#include "idevice_resource.h"
#include <utils/path.h>

namespace chroma { namespace device {

class IDeviceProgramShader : public IDeviceResource {
public:
    enum Type {
        VertexShader,
        GeometryShader,
        TessEvalShader,
        TessControlShader,
        FragmentShader,
        ComputeShader
    };

    static IDeviceProgramShader* create(IDevice* device, Type type_);
    static void destroy(IDeviceProgramShader* shader);

    virtual bool is_valid() const noexcept = 0;
    virtual Type type() const noexcept = 0;
    virtual IDeviceProgramShader* source_file(const utils::Path& path) noexcept = 0;
    virtual IDeviceProgramShader* source_code(const std::string& src) noexcept = 0;
    virtual bool compile() noexcept = 0;
    virtual std::string compile_error() const noexcept = 0;
};

}} // namespace chroma -> device

#endif