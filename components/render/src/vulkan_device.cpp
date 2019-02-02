#include "vulkan_device.h"
#include <memory>
#include "vulkan_instance.h"
#include "vulkan_adequacy.h"
#include <system/c_str.h>

namespace render { namespace vk {

struct Device::Intermediate {
    VkInstance instance = VK_NULL_HANDLE;
    std::vector<std::string> extensions;
    std::vector<std::string> layers;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkPhysicalDeviceFeatures features = {};
    Properties<VkPhysicalDevice>::QueueFamilyIndices queue_families;
    std::vector<VkDeviceQueueCreateInfo> queues;
};

Device::Device(Instance* inst,
               VkSurfaceKHR surface,
               const std::vector<std::string>& extensions,
               VkPhysicalDeviceFeatures features)
    : Properties(VK_NULL_HANDLE) {
    Intermediate interm;
    interm.instance = (VkInstance)(*inst);
    interm.extensions = extensions;
    interm.surface = surface;
    interm.layers = inst->enabled_extensions();

    choose_physical_device(&interm);
    choose_queue_families(&interm);
    create_logical_device(&interm);
    get_device_queues(&interm);
}

void Device::choose_physical_device(Intermediate* interm) {
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(interm->instance, &count, nullptr);
    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(interm->instance, &count, devices.data());
    if (count <= 0)
        throw std::runtime_error("No physical device available.");
    
    for (size_t i = 0; i < devices.size(); i++) {
        if (Adequacy<VkPhysicalDevice>(devices[i])
                .require_extensions(interm->extensions)
                .support(interm->surface)
                .require_queue_family(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)
                .is_satiable()) {
            m_physical_device = devices[i];
            Properties<VkPhysicalDevice>::reset(m_physical_device);
            return;
        }
    }

    if (m_physical_device == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find suitable physical device.");
    }
}

void Device::choose_queue_families(Intermediate* interm) {
    auto indices = Properties<VkPhysicalDevice>::queue_family_indices(interm->surface);
    if (!indices.is_valid()) {
        throw std::runtime_error("Failed to find required queue families.");
        return;
    }

    std::set<INDEX> unique_families = { indices.graphics, indices.present, indices.compute };
    float priority = 1.0f;
    
    for (INDEX index : unique_families) {
        VkDeviceQueueCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.queueFamilyIndex = (uint32_t)index;
        info.queueCount = 1;
        info.pQueuePriorities = &priority;
        interm->queues.push_back(info);
    }
}

void Device::create_logical_device(Intermediate* interm) {
    sys::StringArrayDeleter deleter;
    VkDeviceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    info.pQueueCreateInfos = interm->queues.data();
    info.queueCreateInfoCount = (uint32_t)interm->queues.size();
    info.pEnabledFeatures = &interm->features;

    info.ppEnabledExtensionNames = sys::cstr_array(interm->extensions, &deleter);
    info.enabledExtensionCount = (uint32_t)interm->extensions.size();

    info.ppEnabledLayerNames = sys::cstr_array(interm->layers, &deleter);
    info.enabledLayerCount = (uint32_t)interm->layers.size();

    VkResult err = vkCreateDevice(m_physical_device, &info, nullptr, &m_device);
    if (err != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device.");
        return;
    }
}

void Device::get_device_queues(Intermediate* interm) {
    vkGetDeviceQueue(m_device, interm->queue_families.graphics, 0, &m_graphics_queue);
    vkGetDeviceQueue(m_device, interm->queue_families.present, 0, &m_present_queue);
    vkGetDeviceQueue(m_device, interm->queue_families.compute, 0, &m_compute_queue);
}

Device::operator bool() const {
    return is_valid();
}

bool Device::is_valid() const {
    return m_device != VK_NULL_HANDLE;
}

bool Device::operator<(const Device& other) const {
    return m_device < (VkDevice)other;
}

bool Device::operator==(const Device& other) const {
    return m_device == (VkDevice)other;
}

Device::operator VkPhysicalDevice() const {
    return m_physical_device;
}

Device::operator VkDevice() const {
    return m_device;
}

VkQueue Device::graphics_queue() const {
    return m_graphics_queue;
}

VkQueue Device::present_queue() const {
    return m_present_queue;
}

VkQueue Device::compute_queue() const {
    return m_compute_queue;
}

}} // namespace render -> vk