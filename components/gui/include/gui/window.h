#ifndef CHROMA_UI_WINDOW_H
#define CHROMA_UI_WINDOW_H

#include <iostream>
#include <string>
#include <math/vec2.h>
#include <gui/event.h>
#include <memory>

struct GLFWwindow;

namespace gui {

class Window : public std::enable_shared_from_this<Window> {
    friend class Builder;
public:
    class Builder {
    public:
        Builder& title(const char* str);
        Builder& size(const math::int2& wh);
        Builder& resizable(bool enabled);
        Builder& gl_version(int major, int minor);
        Builder& depth_bits(int bits);
        Builder& stencil_bits(int bits);
        std::shared_ptr<Window> create();

    private:
        const char* m_title = "Untitled";
        int m_width = 1000;
        int m_height = 618;
        bool m_resizable = true;
        int m_gl_major = 3;
        int m_gl_minor = 3;
        int m_depth_bits = 24;
        int m_stencil_bits = 8;
    };

    static bool init();
    static void shutdown();

    Window(GLFWwindow* window = nullptr);
    virtual ~Window();

    math::int2 size() const;
    void resize(const math::int2& s);
    math::int2 pos() const;
    void set_pos(const math::int2& pos);
    void move(const math::int2& diff);
    void set_title(const char* name);
    void main_loop();

protected:
    virtual void mouse_event(const MouseEvent& event) {}
    virtual void key_event(const KeyEvent& event) {}
    virtual void resize_event(const ResizeEvent& event) {}
    virtual void show_event(const ShowEvent& event){}

private:
    GLFWwindow* m_window = nullptr;
};

} // namespace window

#endif