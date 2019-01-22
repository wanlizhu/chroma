#ifndef CHROMA_IDEVICE_H
#define CHROMA_IDEVICE_H

#include <utils/compiler.h>
#include "idevice_attribute.h"
#include "idevice_buffer.h"
#include "idevice_program.h"
#include <idevice_texture.h>
#include <memory>
#include <unordered_map>
#include <vector>

namespace chroma { namespace device {

class IDevice : public IDeviceAttribute {
public:
    enum Type {
        OpenGL,
        Vulkan,
    };

    static IDevice* create(Type type_);
    static void destroy(IDevice* instance);

    virtual Type type() const noexcept = 0;
    virtual DeviceProgram* program() noexcept = 0;
    virtual void install(DeviceProgram* program_) noexcept = 0;
    virtual void use(const std::string& name) noexcept = 0;
};

}} // namespace chroma -> device

#endif