#ifndef CHROMA_UI_WINDOW_H
#define CHROMA_UI_WINDOW_H

#include <iostream>
#include <string>
#include <numeric/vec2.h>
#include <display/event.h>
#include <memory>
#include <unordered_map>

struct GLFWwindow;

namespace gui {

    class Window : public std::enable_shared_from_this<Window> {
        friend class Builder;
    public:
        typedef void(*FUNC_ON_RESIZE)(int width, int height);
        typedef void(*FUNC_ON_REDRAW)();
        typedef void(*FUNC_ON_MOUSE)(const MouseEvent& event);
        typedef void(*FUNC_ON_KEY)(const KeyEvent& event);
        typedef void(*FUNC_ON_DROP)(const DropEvent& event);

        class Builder {
        public:
            Builder& title(const char* str);
            Builder& size(const numeric::int2& wh);
            Builder& resizable(bool enabled);
            Builder& gl_version(int major, int minor);
            Builder& depth_bits(int bits);
            Builder& stencil_bits(int bits);
            Builder& decorated(bool enabled);
            Builder& visible(bool enabled);
            Builder& srgb(bool enabled);
            std::shared_ptr<Window> create();

        private:
            const char* m_title = "Untitled";
            int m_width = 1000;
            int m_height = 618;
            bool m_resizable = true;
            bool m_visible = true;
            bool m_decorated = true;
            bool m_srgb = true;
            int m_gl_major = 3;
            int m_gl_minor = 3;
            int m_depth_bits = 24;
            int m_stencil_bits = 8;
        };

    public:
        static bool init();
        static void shutdown();

        Window(GLFWwindow* window = nullptr);
        virtual ~Window();

        numeric::int2 size() const;
        void resize(const numeric::int2& s);
        numeric::int2 pos() const;
        void set_pos(const numeric::int2& pos);
        void move(const numeric::int2& diff);
        void set_title(const char* name);

        void main_loop();
        void set_on_resize(FUNC_ON_RESIZE func);
        void set_on_redraw(FUNC_ON_REDRAW func);
        void set_on_mouse(FUNC_ON_MOUSE func);
        void set_on_key(FUNC_ON_KEY func);
        void set_on_drop(FUNC_ON_DROP func);

    protected:
        static void glfw_framesize_cb(GLFWwindow* window, int w, int h);
        static void glfw_mousebutton_cb(GLFWwindow* window, int button, int action, int mods);
        static void glfw_cursorpos_cb(GLFWwindow* window, double x, double y);
        static void glfw_scroll_cb(GLFWwindow* window, double x, double y);
        static void glfw_key_cb(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void glfw_drop_cb(GLFWwindow* window, int count, const char** paths);
        static void glfw_error_cb(int code, const char* msg);

    private:
        static int m_instances;
        GLFWwindow* m_window = nullptr;
        struct {
            FUNC_ON_REDRAW on_redraw = nullptr;
            FUNC_ON_RESIZE on_resize = nullptr;
            FUNC_ON_MOUSE on_mouse = nullptr;
            FUNC_ON_KEY on_key = nullptr;
            FUNC_ON_DROP on_drop = nullptr;
        } m_callbacks;
    };

} // namespace window

#endif