#ifndef CHROMA_RENDER_VULKAN_PROC_H
#define CHROMA_RENDER_VULKAN_PROC_H

#include "vulkan_types.h"
#include <string>

namespace render { namespace vk {

PFN_vkVoidFunction dynamic_proc(VkInstance instance, const std::string& name, bool except = true);
PFN_vkVoidFunction dynamic_proc(VkDevice device, const std::string& name, bool except = true);

}} // namespace render -> vk
                   
#define VulkanDynamicProc(target, name) reinterpret_cast<PFN_##name>(render::vk::dynamic_proc(target, #name))

#endif