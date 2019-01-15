#ifndef CHROMA_IBUFFER_H
#define CHROMA_IBUFFER_H

#include <stdint.h>

namespace chroma {

class IBuffer {
public:
    virtual size_t size() const noexcept = 0;
    virtual char* begin() noexcept = 0;
    virtual const char* begin() const noexcept = 0;
    virtual char* end() noexcept = 0;
    virtual const char* end() const noexcept = 0;

    virtual bool read(size_t offset, size_t size_, void* dst) const noexcept = 0;
    virtual bool write(size_t offset, size_t size_, const void* src) noexcept = 0;
};

}

#endif