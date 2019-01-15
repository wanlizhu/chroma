#ifndef CHROMA_IDEVICE_STATE_H
#define CHROMA_IDEVICE_STATE_H

#include <stdint.h>

namespace chroma {

class IDeviceState {
    virtual ~IDeviceState() = default;

    virtual void push() = 0;
    virtual void pop() = 0;
};

class IBlendState : public IDeviceState {
protected:
    virtual ~IBlendState() = default;
};

class IDepthState : public IDeviceState {
protected:
    virtual ~IDepthState() = default;
};

class IRasterState : public IDeviceState {
protected:
    virtual ~IRasterState() = default;
};

class ISamplerState : public IDeviceState {
protected:
    virtual ~ISamplerState() = default;
};

}

#endif