#include "vulkan_api.h"
#include <iostream>

namespace render { namespace device { 

std::string physical_device_type(VkPhysicalDeviceType type)
{
#define TYPE_STRING(r) case VK_PHYSICAL_DEVICE_TYPE_ ##r: return #r
    switch (type) {
        TYPE_STRING(OTHER);
        TYPE_STRING(INTEGRATED_GPU);
        TYPE_STRING(DISCRETE_GPU);
        TYPE_STRING(VIRTUAL_GPU);
        default: return "UNKNOWN_DEVICE_TYPE";
    }
#undef TYPE_STRING
}

bool create_instance(VkInstance* instance,
                  const std::string& name, 
                  const std::vector<const char*>& exts,
                  const std::vector<const char*>& layers,
                  const VkAllocationCallbacks* allocator) {
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = name.c_str();
    app_info.pEngineName = name.c_str();
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = (uint32_t)exts.size();
    inst_info.ppEnabledExtensionNames = exts.data();
    inst_info.enabledLayerCount = (uint32_t)layers.size();
    inst_info.ppEnabledLayerNames = layers.data();

    VkInstance instance = {};
    VkResult res = vkCreateInstance(&inst_info, allocator, instance);
    if (res != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance : " << error(res) << std::endl;
        return false;
    }

    return true;
}

const char* error(int code) {
#define ERROR_STRING(r) case VK_##r: return #r
    switch (code) {
        ERROR_STRING(NOT_READY);
        ERROR_STRING(TIMEOUT);
        ERROR_STRING(EVENT_SET);
        ERROR_STRING(EVENT_RESET);
        ERROR_STRING(INCOMPLETE);
        ERROR_STRING(ERROR_OUT_OF_HOST_MEMORY);
        ERROR_STRING(ERROR_OUT_OF_DEVICE_MEMORY);
        ERROR_STRING(ERROR_INITIALIZATION_FAILED);
        ERROR_STRING(ERROR_DEVICE_LOST);
        ERROR_STRING(ERROR_MEMORY_MAP_FAILED);
        ERROR_STRING(ERROR_LAYER_NOT_PRESENT);
        ERROR_STRING(ERROR_EXTENSION_NOT_PRESENT);
        ERROR_STRING(ERROR_FEATURE_NOT_PRESENT);
        ERROR_STRING(ERROR_INCOMPATIBLE_DRIVER);
        ERROR_STRING(ERROR_TOO_MANY_OBJECTS);
        ERROR_STRING(ERROR_FORMAT_NOT_SUPPORTED);
        ERROR_STRING(ERROR_SURFACE_LOST_KHR);
        ERROR_STRING(ERROR_NATIVE_WINDOW_IN_USE_KHR);
        ERROR_STRING(SUBOPTIMAL_KHR);
        ERROR_STRING(ERROR_OUT_OF_DATE_KHR);
        ERROR_STRING(ERROR_INCOMPATIBLE_DISPLAY_KHR);
        ERROR_STRING(ERROR_VALIDATION_FAILED_EXT);
        ERROR_STRING(ERROR_INVALID_SHADER_NV);
        default:
        return "UNKNOWN_ERROR";
    }
#undef ERROR_STRING
}

std::vector<Device> device_list(VkInstance instance, bool print) {
    std::vector<VkPhysicalDevice> devices;

    uint32_t count = 0;
    vkEnumeratePhysicalDevices(instance, &count, nullptr);
    if (count <= 0) return std::vector<Device>();

    devices.resize(count);
    VkResult err = vkEnumeratePhysicalDevices(instance, &count, devices.data());
    if (err != VK_SUCCESS) {
        std::cerr << "Failed to enumerate physical devices : " << error(err) << std::endl;
        return std::vector<Device>();
    }

    if (print) {
        VkPhysicalDeviceProperties device_props;
        for (uint32_t i = 0; i < count; i++) {
            vkGetPhysicalDeviceProperties(devices[i], &device_props);

            std::cout << "Device [" << i << "] : " << device_props.deviceName << std::endl;
            std::cout << "    Type : " << physical_device_type(device_props.deviceType) << std::endl;
            std::cout << "    API : " 
                << (device_props.apiVersion >> 22) 
                << "." << ((device_props.apiVersion >> 12) & 0x3ff)
                << "." << (device_props.apiVersion & 0xfff)
                << std::endl;
        }
    }

    std::vector<Device> res(count);
    for (uint32_t i = 0; i < count; i++) {
        res[i].physical_device = devices[i];
        vkGetPhysicalDeviceProperties(res[i].physical_device, &res[i].props);
        vkGetPhysicalDeviceMemoryProperties(res[i].physical_device, &res[i].memprops);
        vkGetPhysicalDeviceFeatures(res[i].physical_device, &res[i].features);

        if (print) {
            std::cout << "Device [" << i << "] : " << res[i].props.deviceName << std::endl;
            std::cout << "    Type : " << physical_device_type(res[i].props.deviceType) << std::endl;
            std::cout << "    API : "
                << (res[i].props.apiVersion >> 22)
                << "." << ((res[i].props.apiVersion >> 12) & 0x3ff)
                << "." << (res[i].props.apiVersion & 0xfff)
                << std::endl;
        }
    }

    return res;
}

}} // namespace render -> device 