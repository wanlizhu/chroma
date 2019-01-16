#ifndef CHROMA_IDEVICE_PROGRAM_INPUT_H
#define CHROMA_IDEVICE_PROGRAM_INPUT_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <functional>
#include "idevice_program_input_slot.h"

namespace chroma {

class IDevice;

class IDeviceProgramInput {
public:
    typedef IDeviceProgramInputSlot Slot;
    typedef std::function<bool(const Slot*)> PredicateFunc;
    enum Schema {
        ChromaPBR
    };

    virtual bool is_valid() const noexcept = 0;
    virtual Schema schema() const noexcept = 0;
    virtual Slot* slot(int index) noexcept = 0;
    virtual Slot* slot(const std::string& name) noexcept = 0;
    virtual std::vector<Slot*> slots_if(const PredicateFunc& predicate) noexcept = 0;
    virtual int slot_count() const noexcept = 0;

protected:
    virtual bool add_slot(Slot* slot) noexcept = 0;
    virtual void remove_slot(Slot* slot) noexcept = 0;
};

}

#endif