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
    typedef IDeviceBuffer Buffer;
    typedef IDeviceProgram Program;
    typedef IDeviceTexture Texture;

    static IDevice* create();
    static void destroy(IDevice* device);

    virtual void install(IDeviceProgram* program_) noexcept = 0;
    virtual void use(const std::string& name) noexcept = 0;
    virtual IDeviceProgram* program() noexcept = 0;
};

}} // namespace chroma -> device

#endif