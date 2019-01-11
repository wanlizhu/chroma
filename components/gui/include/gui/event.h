#ifndef CHROMA_GUI_EVENT_H
#define CHROMA_GUI_EVENT_H

#include <math/vec2.h>

#ifdef MOD_ALT
#undef MOD_ALT
#endif
#ifdef MOD_SHIFT
#undef MOD_SHIFT
#endif
#ifdef MOD_CONTROL
#undef MOD_CONTROL
#endif

namespace gui {

enum EventEnum {
    MOUSE_LEFT,
    MOUSE_RIGHT,
    ACTION_DOWN,
    ACTION_UP,
    ACTION_DOUBLE,
    ACTION_HOLD,
    MOD_CTRL,
    MOD_SHIFT,
    MOD_ALT,
    KEY_ESC,
    KEY_ENTER,
};

struct IEvent {
    virtual ~IEvent() = default;
};

struct MouseEvent : public IEvent {
    struct {
        bool valid = false;
        double x, y;
        int mods = 0;
    } wheel, move;

    struct {
        bool valid = false;
        EventEnum id;
        EventEnum action;
        int mods = 0;
    } button;


    virtual ~MouseEvent() = default;
};

struct KeyEvent : public IEvent {
    int key = 0;
    EventEnum action;
    int mods = 0;

    virtual ~KeyEvent() = default;
};

struct ResizeEvent : public IEvent {
    int width;
    int height;

    virtual ~ResizeEvent() = default;
};

struct ShowEvent : public IEvent {
    virtual ~ShowEvent() = default;
};

}

#endif