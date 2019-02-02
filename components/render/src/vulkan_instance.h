#ifndef CHROMA_RENDER_VULKAN_INSTANCE_H
#define CHROMA_RENDER_VULKAN_INSTANCE_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>
#include "vulkan_validation_layer.h"
#include "vulkan_properties.h"
#include <system/noncopyable.h>

namespace render { namespace vk {  

class Instance : public ValidationLayer
               , public Properties<VkInstance>
               , public sys::NonMovable {
public:
    static std::vector<std::string> BASIC_EXTENSIONS;
    static std::vector<std::string> BASIC_LAYERS;

public:
    Instance(VkInstance inst);
    Instance(const std::vector<std::string>& extensions,
             const std::vector<std::string>& layers);
    virtual ~Instance();

    bool operator==(const Instance& other) const;
    bool operator<(const Instance& other) const;
    operator bool() const;
    operator VkInstance() const;
    void destroy();
    bool is_valid() const;
    std::vector<VkPhysicalDevice> physical_devices() const;

private:
    VkInstance m_instance = VK_NULL_HANDLE;
};

}} // namespace render -> vk

#endif