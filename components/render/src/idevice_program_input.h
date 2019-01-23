#ifndef CHROMA_IDEVICE_PROGRAM_INPUT_H
#define CHROMA_IDEVICE_PROGRAM_INPUT_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <functional>
#include "idevice_program_input_slot.h"

namespace render { namespace device {

class IDevice;

class IDeviceProgramInput {
    friend class IDeviceProgram;
public:
    typedef IDeviceProgramInputSlot Slot;
    enum Schema {
        ChromaPBR
    };

    bool is_valid() const noexcept;
    Schema schema() const noexcept;
    Slot* slot(const std::string& name) noexcept;

protected:
    bool connect(Slot* slot) noexcept;
    void disconnect(Slot* slot) noexcept;

};

}} // namespace render -> device

#endif