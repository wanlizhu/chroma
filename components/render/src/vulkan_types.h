#ifndef CHROMA_RENDER_TYPES_H
#define CHROMA_RENDER_TYPES_H

#include <string>
#include <vector>
#include <map>
#include <numeric/vec4.h>

#include <vulkan/vulkan.h>
#if defined(WINDOWS)
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#elif defined(MACOS)
#include <vulkan/vulkan_macos.h>
#elif defined(IOS)
#include <vulkan/vulkan_ios.h>
#elif defined(ANDROID)
#include <vulkan/vulkan_android.h>
#endif

namespace render { namespace vk {

typedef uint32_t INDEX;
typedef uint64_t OBJECT;
typedef uint64_t TAGNAME;

static const INDEX NULL_INDEX = (INDEX)-1;

}} // namespace render -> vk

#endif