#ifndef CHROMA_DEVICE_RESOURCE_H
#define CHROMA_DEVICE_RESOURCE_H

#include <stdint.h>
#include <memory>

namespace render { namespace device {

class IDeviceResource {
public:
    enum Type {
        Undefined,
        Buffer,
        RenderBuffer,
        Texture,
        Sampler
    };

    virtual bool create() const noexcept = 0;
    virtual void sync() const noexcept = 0;
    virtual void destroy() const noexcept = 0;

    inline int id() const noexcept { return m_id; }
    inline Type type() const noexcept { return m_type; }
    inline bool is_valid() const noexcept { return m_id != -1; }
    inline bool is_synced() const noexcept { return !m_sync_later; }

protected:
    IDeviceResource(Type type_, int id_)
        : m_type(type_), m_id(id_) 
    {}
    inline void set_id(int id_) noexcept { m_id = id_; }
    inline void sync_later() noexcept { m_sync_later = true; };
    
private:
    Type m_type = Type::Undefined;
    bool m_sync_later = false;
    int m_id = -1;
};

}} // namespace render -> device

#endif