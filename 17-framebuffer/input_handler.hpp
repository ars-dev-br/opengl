#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <GLFW/glfw3.h>

class InputHandler {
private:
    GLFWwindow* window;

public:
    InputHandler(GLFWwindow* window)
        : window(window) {
    }

    void poll() {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
};

#endif
