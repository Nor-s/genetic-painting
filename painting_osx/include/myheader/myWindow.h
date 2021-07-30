#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <map>

#include "learnopengl/stb_image_write.h"


namespace nsg {
    class myWindow{
    private:
        static bool drawingSemaphore;

        GLFWwindow* window;
        int currentWidth;
        int currentHeight;

        unsigned int PBO[2];
    public:        
        static int SCR_WIDTH;
        static int SCR_HEIGHT;
        static int size;
        static glm::mat4 projection;
        static std::map<int, myWindow*> windowDictionary;
        static void drawingLock();
        static void drawingUnLock();
        
        void screenshot(GLubyte** pboMem);
        myWindow(int width, int height, const char* title);
        GLFWwindow* initWindow(int width, int height, const char* name);
        GLFWwindow* getWindow();
        void createWindow(int width, int height, const char* title);
        void deleteWindow();
        void initPBO();
        void windowClear(GLbitfield mask, GLfloat r, GLfloat g, GLfloat b, GLfloat w);
        GLubyte** getWindowPixel();
        void windowCapture(const char* filePath);
    };

}
#endif