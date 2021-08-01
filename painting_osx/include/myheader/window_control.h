#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "learnopengl/stb_image_write.h"

namespace nsg
{
    class WindowControl
    {
    private:
        static bool rendering_semaphore_;

        GLFWwindow *window_;
        std::string title_;
        int padding_;
        int file_size_;
        unsigned int pbo_[3];
        int relative_width_;
        int relative_height_;

    public:
        static int byte_per_pixel_;
        static int stride_size_;
        static GLubyte **grayscale_picture_;
        static int g_width_;
        static int g_height_;
        static int size_;
        static std::vector<WindowControl *> g_windows_;
        static void rendering_lock();
        static void rendering_unlock();

        WindowControl(int width, int height, const char *title);
        ~WindowControl();
        GLFWwindow *init_window();
        GLFWwindow *get_window();
        void init_pbo();
        void clear_window_white();
        GLubyte **get_window_halfpixel();
        GLubyte *get_window_fullpixel();
        void window_to_file(const char *filename);
        void byte_to_file(GLubyte **pboMem, const char *filename);
        void set_buffersize_pbo();
        void set_screenshot_size();
        void set_height(int height);
        void set_width(int width);
        int get_relative_height();
        int get_relative_width();

        void resize_window(int width, int height);
        void read_pixels(int x, int y, int width, int height);
    };

}
#endif