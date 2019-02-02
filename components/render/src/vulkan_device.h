#ifndef CHROMA_VULKAN_DEVICE_H
#define CHROMA_VULKAN_DEVICE_H

#include "vulkan_properties.h"
#include <system/noncopyable.h>

namespace render { namespace vk {

class Instance;

class Device : public Properties<VkPhysicalDevice>
             , public sys::NonMovable {
    struct Intermediate;
public:
    Device(Instance* inst, 
           VkSurfaceKHR surface,
           const std::vector<std::string>& extensions,
           VkPhysicalDeviceFeatures features);

    operator bool() const;
    bool is_valid() const;
    bool operator<(const Device& other) const;
    bool operator==(const Device& other) const;

    operator VkPhysicalDevice() const;
    operator VkDevice() const;
    VkQueue graphics_queue() const;
    VkQueue present_queue() const;
    VkQueue compute_queue() const;

private:
    void choose_physical_device(Intermediate* interm);
    void choose_queue_families(Intermediate* interm);
    void create_logical_device(Intermediate* interm);
    void get_device_queues(Intermediate* interm);

private:
    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;
    VkQueue m_graphics_queue = VK_NULL_HANDLE;
    VkQueue m_present_queue = VK_NULL_HANDLE;
    VkQueue m_compute_queue = VK_NULL_HANDLE;
};                                     

}} // namespace render -> vk

#endif