#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace nsg {
    class myWindow{
    private:
        GLFWwindow* window;
        unsigned int SCR_WIDTH;
        unsigned int SCR_HEIGHT;

        unsigned int PBO[2];

    public:
        myWindow();
        GLFWwindow* initWindow();
        GLFWwindow* getWindow();
        void initPBO();
        void windowClear(GLbitfield mask, GLfloat r, GLfloat g, GLfloat b, GLfloat w);
        void windowCapture(const char* filePath);
    };
    void windowResizeCallback(GLFWwindow* windowPointer, int width, int height);
}
#endif