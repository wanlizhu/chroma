#ifndef CHROMA_VULKAN_DEVICE_H
#define CHROMA_VULKAN_DEVICE_H

#include "vulkan_properties.h"
#include <system/noncopyable.h>

namespace render { namespace vk {

class Instance;

class Device : public Properties<VkPhysicalDevice>
             , public sys::NonMovable {
public:
    Device(Instance* inst, VkSurfaceKHR surface, const std::vector<std::string>& extensions,
           VkPhysicalDeviceFeatures features = {});

private:
    Instance* m_instance = nullptr;
    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;
};                                     

}} // namespace render -> vk

#endif