#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "learnopengl/stb_image_write.h"


namespace nsg {
    class myWindow{
    private:
        GLFWwindow* window;
        int currentWidth;
        int currentHeight;

        unsigned int PBO[2];
    public:        
        static int SCR_WIDTH;
        static int SCR_HEIGHT;
        static glm::mat4 projection;
        
        myWindow(int width, int height, const char* title);
        GLFWwindow* initWindow(int width, int height, const char* name);
        GLFWwindow* getWindow();
        void initPBO();
        void windowClear(GLbitfield mask, GLfloat r, GLfloat g, GLfloat b, GLfloat w);
        void windowCapture(const char* filePath);
    };

}
#endif