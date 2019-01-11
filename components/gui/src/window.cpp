#include <glad.h> // Must include before glfw3.h
#include <gui/window.h>
#include <GLFW/glfw3.h>

namespace gui {

namespace details {

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    std::shared_ptr<Window> user(static_cast<Window*>(glfwGetWindowUserPointer(window)));
    
}

} // namespace details

Window::Builder& Window::Builder::title(const char* str) {
    m_title = str;
    return *this;
}

Window::Builder& Window::Builder::size(const math::int2& wh) {
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

std::shared_ptr<Window> Window::Builder::create() {
#ifdef APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_gl_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_gl_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, m_resizable ? 1 : 0);
    glfwWindowHint(GLFW_STENCIL_BITS, m_stencil_bits);
    glfwWindowHint(GLFW_DEPTH_BITS, m_depth_bits);

    GLFWwindow* window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
    if (window == nullptr) {
        throw std::runtime_error("Failed to create GLFW window.");
    }
    std::shared_ptr<Window> ptr = std::make_shared<Window>(window);

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, ptr.get());
    glfwSetFramebufferSizeCallback(window, details::framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    return ptr;
}


bool Window::init() {
    return glfwInit() == GLFW_TRUE;
}

void Window::shutdown() {
    glfwTerminate();
}        

Window::Window(GLFWwindow* window)
    : m_window(window) {

}

Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

math::int2 Window::size() const {
    math::int2 wh;
    glfwGetWindowSize(m_window, &wh[0], &wh[1]);
    return wh;
}

void Window::resize(const math::int2& s) {
    glfwSetWindowSize(m_window, s[0], s[1]);
}

math::int2 Window::pos() const {
    math::int2 p;
    glfwGetWindowPos(m_window, &p[0], &p[1]);
    return p;
}

void Window::set_pos(const math::int2& pos) {
    glfwSetWindowPos(m_window, pos[0], pos[1]);
}

void Window::move(const math::int2& diff) {
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