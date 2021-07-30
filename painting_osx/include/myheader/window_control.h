#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "myheader/myPainting.h"
#include "learnopengl/stb_image_write.h"


namespace nsg {
    class WindowControl{
    private:
        static bool rendering_semaphore_;

        GLFWwindow* window_;
        int width_;
        int height_;
        std::string title_;

        unsigned int pbo_[2];
    public:        
        static GLubyte** grayscale_picture_;
        static myPainting *picture_;
        static int g_width_;
        static int g_height_;
        static int size_;
        static std::vector<WindowControl*> g_windows_;
        static void rendering_lock();
        static void rendering_unlock();
        
        void screenshot(GLubyte** pboMem);
        WindowControl(int width, int height, const char* title);
        GLFWwindow* init_window();
        GLFWwindow* get_window();
        void init_pbo();
        void window_clear_white();
        GLubyte** get_window_pixel();
        void window_capture(const char* filePath);
    };

}
#endif