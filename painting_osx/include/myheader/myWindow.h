#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "myheader/myCallback.h"

#include <iostream>

namespace nsg {
    class myWindow{
    private:
        GLFWwindow* window;
        unsigned int SCR_WIDTH;
        unsigned int SCR_HEIGHT;

    public:
        myWindow();
        GLFWwindow* initWindow();
        GLFWwindow* getWindow();
        void windowClear(GLbitfield mask, GLfloat r, GLfloat g, GLfloat b, GLfloat w);
    };
}
#endif