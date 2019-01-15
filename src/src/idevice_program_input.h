#ifndef CHROMA_IDEVICE_PROGRAM_INPUT_H
#define CHROMA_IDEVICE_PROGRAM_INPUT_H

#include <stdint.h>
#include <memory>

namespace chroma {

class IDevice;

class IDeviceProgramInput : public std::enable_shared_from_this<IDeviceProgramInput> {
public:
    static std::shared_ptr<IDeviceProgramInput> create(std::shared_ptr<IDevice> device);

protected:
    IDeviceProgramInput() = default;
    IDeviceProgramInput(const IDeviceProgramInput&) = default;
    IDeviceProgramInput& operator=(const IDeviceProgramInput&) = default;
    virtual ~IDeviceProgramInput() = default;
};

}

#endif