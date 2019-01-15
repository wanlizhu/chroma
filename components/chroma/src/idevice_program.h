#ifndef CHROMA_IDEVICE_PROGRAM_H
#define CHROMA_IDEVICE_PROGRAM_H

#include <stdint.h>
#include <memory>
#include "idevice_program_input.h"
#include "idevice_program_output.h"

namespace chroma {

class IDevice;

class IDeviceProgram : public std::enable_shared_from_this<IDeviceProgram> {
public:
    class Builder {
        struct SourceData;
    public:

    private:
        SourceData* m_data = nullptr;
    };

    inline std::shared_ptr<IDeviceProgramInput>& input() noexcept { return m_input; }
    inline std::shared_ptr<IDeviceProgramOutput>& output() noexcept { return m_output; }

protected:
    IDeviceProgram() = default;
    IDeviceProgram(const IDeviceProgram&) = default;
    IDeviceProgram& operator=(const IDeviceProgram&) = default;
    virtual ~IDeviceProgram() = default;

protected:
    std::shared_ptr<IDeviceProgramInput> m_input;
    std::shared_ptr<IDeviceProgramOutput> m_output;
};

}

#endif