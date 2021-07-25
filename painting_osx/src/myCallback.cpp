#include "myheader/myWindow.h"

namespace nsg {
    void processInput(GLFWwindow* windowPointer) {
        if (glfwGetKey(windowPointer, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(windowPointer, true);
        }
    }
    
    void windowResizeCallback(GLFWwindow* windowPointer, int width, int height) {
        glViewport(0, 0, width, height);
    }
}