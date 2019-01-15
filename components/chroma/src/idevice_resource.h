#ifndef CHROMA_IDEVICE_RESOURCE_H
#define CHROMA_IDEVICE_RESOURCE_H

#include <stdint.h>

namespace chroma {

class IDeviceResource {
public:
    IDeviceResource(int id_ = -1) : m_id(id_) {}
    virtual ~IDeviceResource() = default;

    inline int id() const noexcept { return m_id; }
    inline bool is_valid() const noexcept { return m_id != -1; }

protected:
    inline void set_id(int id_) noexcept { m_id = id_; }

private:
    int m_id = -1;
};

}

#endif