#ifndef CHROMA_IDEVICE_COLOR_FORMAT_H
#define CHROMA_IDEVICE_COLOR_FORMAT_H

#include <stdint.h>
#include "idevice_resource.h"
#include <type_traits>
#include <memory>
#include <typeinfo>
#include "idevice_basic_types.h"

namespace chroma { namespace device {

class IDevice;

typedef size_t IDeviceColorFormatID;

template<int CHANNELS, typename CHANNEL_TYPE,
         typename = std::enable_if<device::is_device_scalar<CHANNEL_TYPE>::value> >
class IDeviceColorFormat {
    typedef CHANNEL_TYPE ChannelType;
    static constexpr const int Channels = CHANNELS;
    static constexpr const bool HasAlpha = (CHANNELS == 4);

    static IDeviceColorFormatID id() const noexcept { return typeid(this).hash_code(); }
};

typedef IDeviceColorFormat<3, UINT8> SRGB;
typedef IDeviceColorFormat<4, UINT8> SRGBA;

typedef IDeviceColorFormat<3, FLOAT32> F32_RGB;
typedef IDeviceColorFormat<3, UINT32> U32_RGB;
typedef IDeviceColorFormat<3, UINT8> U8_RGB;

typedef IDeviceColorFormat<4, FLOAT32> F32_RGBA;
typedef IDeviceColorFormat<4, UINT32> U32_RGBA;
typedef IDeviceColorFormat<4, UINT8> U8_RGBA;

typedef IDeviceColorFormat<1, UINT8> STENCIL;
typedef IDeviceColorFormat<1, FLOAT32> DEPTH;

}} // namespace chroma -> device

#endif