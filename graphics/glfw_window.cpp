#include "./glfw_window.h"

GLFW_Window::GLFW_Window(int width, int height)
    : m_width(width), m_height(height)
{
}

GLFW_Window::~GLFW_Window() {
    glfwDestroyWindow(this->m_window);
}

auto GLFW_Window::create() -> void {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    m_window = glfwCreateWindow(m_width, m_height, "Fluid Flow", NULL, NULL);

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error: " << description << std::endl;
    });

    if (!m_window) {
        glfwTerminate();
        std::cerr << "Window or context creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(m_window, [](auto* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    });

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);

    m_initialized = true;
}


auto GLFW_Window::poll_events() -> void {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}


[[nodiscard]] auto GLFW_Window::is_initialized() const -> bool {
    return m_initialized;
}


[[nodiscard]] auto GLFW_Window::should_close() const -> bool {
    return glfwWindowShouldClose(m_window) == GLFW_TRUE;
}