#ifndef MY_CALLBACK_H
#define MY_CALLBACK_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace nsg {
    void windowResizeCallback(GLFWwindow *windowPointer, int width, int height);
    void processInput(GLFWwindow *windowPointer);
}
#endif