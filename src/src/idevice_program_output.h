#ifndef CHROMA_IDEVICE_PROGRAM_OUTPUT_H
#define CHROMA_IDEVICE_PROGRAM_OUTPUT_H

#include <stdint.h>
#include <memory>

namespace chroma {

class IDevice;

class IDeviceProgramOutput : public std::enable_shared_from_this<IDeviceProgramOutput> {
public:
    static std::shared_ptr<IDeviceProgramOutput> create(std::shared_ptr<IDevice> device);

protected:
    IDeviceProgramOutput() = default;
    IDeviceProgramOutput(const IDeviceProgramOutput&) = default;
    IDeviceProgramOutput& operator=(const IDeviceProgramOutput&) = default;
    virtual ~IDeviceProgramOutput() = default;
};

}

#endif