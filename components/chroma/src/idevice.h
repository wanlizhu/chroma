#ifndef CHROMA_IDEVICE_H
#define CHROMA_IDEVICE_H

#include <utils/compiler.h>
#include "idevice_state.h"
#include "idevice_target.h"
#include "idevice_attribute.h"
#include "idevice_buffer.h"
#include "idevice_program.h"
#include <idevice_texture.h>
#include <memory>
#include <unordered_map>
#include <vector>

namespace chroma {
    
class IDevice 
    : public IDeviceAttribute
    , public std::enable_shared_from_this<IDevice> {
public:
    static IDevice* create();
    static void destroy(IDevice* device);

    inline IDeviceProgram*& current_program() noexcept { return m_current_program; }
    inline IBlendState*& blend_state() noexcept { return m_blend_state; }
    inline IDepthState*& depth_state() noexcept { return m_depth_state; }
    inline IRasterState*& raster_state() noexcept { return m_raster_state; }
    inline ISamplerState*& sampler_state() noexcept { return m_sampler_state; }

    inline int target_count() const noexcept { return static_cast<int>(m_targets.size()); }
    inline int program_count() const noexcept { return static_cast<int>(m_targets.size()); }
    inline int buffer_count() const noexcept { return static_cast<int>(m_targets.size()); }
    inline int texture_count() const noexcept { return static_cast<int>(m_targets.size()); }

protected:
    IDevice() = default;
    IDevice(const IDevice&) = default;
    IDevice& operator=(const IDevice&) = default;
    virtual ~IDevice() = default;

protected:
    IDeviceProgram* m_current_program;
    IBlendState* m_blend_state;
    IDepthState* m_depth_state;
    IRasterState* m_raster_state;
    ISamplerState* m_sampler_state;

    std::vector<IDeviceTarget*> m_targets;
    std::vector<IDeviceProgram*> m_programs;
    std::vector<IDeviceBuffer*> m_buffers;
    std::vector<IDeviceTexture*> m_textures;
};

}

#endif