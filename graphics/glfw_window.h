#ifndef _GLFW_WINDOW_
#define _GLFW_WINDOW_

#include <iostream>
#include <GLFW/glfw3.h>

class GLFW_Window {
   private:
        GLFWwindow* m_window;
        int m_width, m_height;

        bool m_initialized;

    public:
        GLFW_Window(int width, int height);
        ~GLFW_Window();

        auto create() -> void;
        auto poll_events() -> void;

        [[nodiscard]] auto is_initialized() const -> bool;
        [[nodiscard]] auto should_close() const -> bool;
};

#endif