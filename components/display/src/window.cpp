#include <glad.h> // Must include before glfw3.h
#include <display/window.h>
#include <GLFW/glfw3.h>

namespace gui {

    int Window::m_instances = 0;

    Window::Builder& Window::Builder::title(const char* str) {
        m_title = str;
        return *this;
    }

    Window::Builder& Window::Builder::size(const numeric::int2& wh) {
        m_width = wh[0];
        m_height = wh[1];
        return *this;
    }

    Window::Builder& Window::Builder::resizable(bool enabled) {
        m_resizable = enabled;
        return *this;
    }

    Window::Builder& Window::Builder::gl_version(int major, int minor) {
        m_gl_major = major;
        m_gl_minor = minor;
        return *this;
    }

    Window::Builder& Window::Builder::depth_bits(int bits) {
        m_depth_bits = bits;
        return *this;
    }

    Window::Builder& Window::Builder::stencil_bits(int bits) {
        m_stencil_bits = bits;
        return *this;
    }

    Window::Builder& Window::Builder::decorated(bool enabled) {
        m_decorated = enabled;
        return *this;
    }

    Window::Builder& Window::Builder::visible(bool enabled) {
        m_visible = enabled;
        return *this;
    }

    Window::Builder& Window::Builder::srgb(bool enabled) {
        m_srgb = enabled;
        return *this;
    }

    std::shared_ptr<Window> Window::Builder::create() {
        Window::init();

    #ifdef APPLE
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    #endif
    #ifndef NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #endif
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_gl_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_gl_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, m_resizable ? 1 : 0);
        glfwWindowHint(GLFW_STENCIL_BITS, m_stencil_bits);
        glfwWindowHint(GLFW_DEPTH_BITS, m_depth_bits);
        glfwWindowHint(GLFW_VISIBLE, m_visible);
        glfwWindowHint(GLFW_DECORATED, m_decorated);
        glfwWindowHint(GLFW_SRGB_CAPABLE, m_srgb);

        GLFWwindow* window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
        if (window == nullptr) {
            throw std::runtime_error("Failed to create GLFW window.");
        }
        std::shared_ptr<Window> ptr = std::make_shared<Window>(window);

        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, ptr.get());
        glfwSetFramebufferSizeCallback(window, &Window::glfw_framesize_cb);
        glfwSetKeyCallback(window, &Window::glfw_key_cb);
        glfwSetMouseButtonCallback(window, &Window::glfw_mousebutton_cb);
        glfwSetScrollCallback(window, &Window::glfw_scroll_cb);
        glfwSetCursorPosCallback(window, &Window::glfw_cursorpos_cb);
        glfwSetDropCallback(window, &Window::glfw_drop_cb);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD.");
        }

        return ptr;
    }

    void Window::set_on_resize(FUNC_ON_RESIZE func) {
        m_callbacks.on_resize = func;
    }

    void Window::set_on_redraw(FUNC_ON_REDRAW func) {
        m_callbacks.on_redraw = func;
    }
    void Window::set_on_mouse(FUNC_ON_MOUSE func) {
        m_callbacks.on_mouse = func;
    }

    void Window::set_on_key(FUNC_ON_KEY func) {
        m_callbacks.on_key = func;
    }

    void Window::set_on_drop(FUNC_ON_DROP func) {
        m_callbacks.on_drop = func;
    }

    void Window::glfw_framesize_cb(GLFWwindow* window, int w, int h) {
        Window* ptr(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (!ptr) return;

        if (ptr->m_callbacks.on_resize) {
            ptr->m_callbacks.on_resize(w, h);
        }
    }

    void Window::glfw_mousebutton_cb(GLFWwindow* window, int button, int action, int mods) {
        Window* ptr(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (!ptr) return;

        MouseEvent ev;
        ev.button.valid = true;
        ev.button.id = button;
        ev.button.action = action;
        ev.button.mods = mods;

        if (ptr->m_callbacks.on_mouse) {
            ptr->m_callbacks.on_mouse(ev);
        }
    }

    void Window::glfw_cursorpos_cb(GLFWwindow* window, double x, double y) {
        Window* ptr(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (!ptr) return;

        MouseEvent ev;
        ev.move.valid = true;
        ev.move.x = x;
        ev.move.y = y;

        if (ptr->m_callbacks.on_mouse) {
            ptr->m_callbacks.on_mouse(ev);
        }
    }

    void Window::glfw_scroll_cb(GLFWwindow* window, double x, double y) {
        Window* ptr(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (!ptr) return;

        MouseEvent ev;
        ev.wheel.valid = true;
        ev.wheel.x = x;
        ev.wheel.y = y;

        if (ptr->m_callbacks.on_mouse) {
            ptr->m_callbacks.on_mouse(ev);
        }
    }

    void Window::glfw_key_cb(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Window* ptr(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (!ptr) return;

        KeyEvent ev;
        ev.key = key;
        ev.action = action;
        ev.mods = mods;

        if (ptr->m_callbacks.on_key) {
            ptr->m_callbacks.on_key(ev);
        }
    }

    void Window::glfw_drop_cb(GLFWwindow* window, int count, const char** paths) {
        Window* ptr(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (!ptr) return;

        DropEvent ev;
        ev.paths.reserve(count);
        for (int i = 0; i < count; i++) {
            ev.paths.push_back(paths[i]);
        }

        if (ptr->m_callbacks.on_drop) {
            ptr->m_callbacks.on_drop(ev);
        }
    }

    void Window::glfw_error_cb(int code, const char* msg) {
        printf("GLFW Error %d : %s\n", code, msg);
    }

    bool Window::init() {
        bool ret = (glfwInit() == GLFW_TRUE);
        glfwSetErrorCallback(&Window::glfw_error_cb);
        return ret;
    }

    void Window::shutdown() {
        glfwTerminate();
    }        

    Window::Window(GLFWwindow* window)
        : m_window(window) {
        m_instances++;
    }

    Window::~Window() {
        if (m_window) {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
        m_instances--;

        if (m_instances <= 0) {
            Window::shutdown();
        }
    }

    numeric::int2 Window::size() const {
        numeric::int2 wh;
        glfwGetWindowSize(m_window, &wh[0], &wh[1]);
        return wh;
    }

    void Window::resize(const numeric::int2& s) {
        glfwSetWindowSize(m_window, s[0], s[1]);
    }

    numeric::int2 Window::pos() const {
        numeric::int2 p;
        glfwGetWindowPos(m_window, &p[0], &p[1]);
        return p;
    }

    void Window::set_pos(const numeric::int2& pos) {
        glfwSetWindowPos(m_window, pos[0], pos[1]);
    }

    void Window::move(const numeric::int2& diff) {
        set_pos(pos() + diff);
    }

    void Window::set_title(const char* name) {
        glfwSetWindowTitle(m_window, name);
    }

    void Window::main_loop() {
        if (!m_window) return;

        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
            //processInput(window);
            glfwSwapBuffers(m_window);
        }
    }

}