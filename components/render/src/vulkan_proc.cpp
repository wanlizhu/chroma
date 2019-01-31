#include "vulkan_proc.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <shared_mutex>
#include <mutex>

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(VkInstance instance,
    const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugReportCallbackEXT* pCallback) {
    PFN_vkCreateDebugReportCallbackEXT proc = VulkanDynamicProc(instance, vkCreateDebugReportCallbackEXT);
    assert(proc != nullptr);
    return proc(instance, pCreateInfo, pAllocator, pCallback);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance,
    VkDebugReportCallbackEXT callback,
    const VkAllocationCallbacks* pAllocator) {
    PFN_vkDestroyDebugReportCallbackEXT proc = VulkanDynamicProc(instance, vkDestroyDebugReportCallbackEXT);
    assert(proc != nullptr);
    return proc(instance, callback, pAllocator);
}

namespace render { namespace vk {

namespace static_data {

template<typename... ARGS>
using HashMap = std::unordered_map<ARGS...>;

HashMap<VkInstance, HashMap<std::string, PFN_vkVoidFunction>> instance_procs;
HashMap<VkDevice, HashMap<std::string, PFN_vkVoidFunction>> device_procs;
std::shared_mutex instance_mutex;
std::shared_mutex device_mutex;

} // namespace data

PFN_vkVoidFunction dynamic_proc(VkInstance instance, const std::string& name, bool except) {
    const static std::string err = "Failed to load Vulkan Instance Proc : ";

    { // try to find it from cache
        std::shared_lock<std::shared_mutex> lock(static_data::instance_mutex);
        if (static_data::instance_procs.find(instance) != static_data::instance_procs.end()) {
            auto it = static_data::instance_procs[instance].find(name);
            if (it != static_data::instance_procs[instance].end()) {
                return it->second;
            }
        }
    }

    // load new proc addr
    auto addr = vkGetInstanceProcAddr(instance, name.c_str());
    if (!addr) {
        if (except) throw std::runtime_error(err + name);
        else return nullptr;
    }

    // insert 
    std::lock_guard<std::shared_mutex> lock(static_data::instance_mutex);
    static_data::instance_procs[instance].insert({ name, addr });

    return addr;
}

PFN_vkVoidFunction dynamic_proc(VkDevice device, const std::string& name, bool except) {
    const static std::string err = "Failed to load Vulkan Device Proc : ";

    { // try to find it from cache
        std::shared_lock<std::shared_mutex> lock(static_data::device_mutex);
        if (static_data::device_procs.find(device) != static_data::device_procs.end()) {
            auto it = static_data::device_procs[device].find(name);
            if (it != static_data::device_procs[device].end()) {
                return it->second;
            }
        }
    }

    // load new proc addr
    auto addr = vkGetDeviceProcAddr(device, name.c_str());
    if (!addr) {
        if (except) throw std::runtime_error(err + name);
        else return nullptr;
    }

    // insert 
    std::lock_guard<std::shared_mutex> lock(static_data::device_mutex);
    static_data::device_procs[device].insert({ name, addr });

    return addr;
}

}} // namespace render -> vk