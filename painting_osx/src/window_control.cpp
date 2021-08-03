#include "myheader/window_control.h"
#define DEBUG_MODE

namespace nsg
{
    std::vector<WindowControl *> WindowControl::g_windows_;
    int WindowControl::size_ = 0;
    bool WindowControl::rendering_semaphore_ = true;
    int WindowControl::g_height_ = 800;
    int WindowControl::g_width_ = 800;
    int WindowControl::byte_per_pixel_ = 3;
    int WindowControl::stride_size_ = 0;
    int WindowControl::file_size_ = 0;
    int WindowControl::relative_height_ = 0;
    int WindowControl::relative_width_ = 0;

    WindowControl::WindowControl(int width, int height, const char *title)
    {
        stbi_flip_vertically_on_write(true);
        title_ = std::string(title);
        set_width(width);
        set_height(height);
        window_ = init_window();
        set_screenshot_size();
        init_pbo();
        size_++;
    }
    WindowControl::~WindowControl()
    {
        glDeleteBuffers(3, pbo_);
        glfwDestroyWindow(window_);
    }
    GLFWwindow *WindowControl::init_window()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // glfw window creation
        // --------------------
        GLFWwindow *window = glfwCreateWindow(g_width_, g_height_, title_.c_str(), NULL, NULL);
        if (window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);
        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return nullptr;
        }

        return window;
    }
    GLFWwindow *WindowControl::get_window()
    {
        if (window_ == nullptr)
        {
            return nullptr;
        }
        return window_;
    }

    void WindowControl::rendering_lock()
    {
        if (!rendering_semaphore_)
        {
            return;
        }
        rendering_semaphore_ = false;
    }
    void WindowControl::rendering_unlock()
    {
        rendering_semaphore_ = true;
    }

    void WindowControl::clear_window_white()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }
    void WindowControl::resize_window(int width, int height)
    {
        set_width(width);
        set_height(height);
        glViewport(0, 0, g_width_, g_height_);
        glfwSetWindowSize(window_, g_width_, g_height_);
        set_screenshot_size();
        set_buffersize_pbo();
    }

    void WindowControl::set_height(int height)
    {
        relative_height_ = g_height_ = height;
#ifdef __APPLE__
        //   relative_height_ *= 2;
#endif
    }
    void WindowControl::set_width(int width)
    {
        relative_width_ = g_width_ = width;
#ifdef __APPLE__
        //    relative_width_ *= 2;
#endif
    }
    int WindowControl::get_relative_height()
    {
        return relative_height_;
    }
    int WindowControl::get_relative_width()
    {
        return relative_width_;
    }
}
/*
    for read pixel
*/
namespace nsg
{

    void WindowControl::init_pbo()
    {
        //pbo gen
        glGenBuffers(3, pbo_);
        set_buffersize_pbo();
    }
    void WindowControl::set_buffersize_pbo()
    {
        int file_size = file_size_;

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[0]);
        glBufferData(GL_PIXEL_PACK_BUFFER, file_size / 2, nullptr, GL_STREAM_READ);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[1]);
        glBufferData(GL_PIXEL_PACK_BUFFER, file_size / 2, nullptr, GL_STREAM_READ);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[2]);
        glBufferData(GL_PIXEL_PACK_BUFFER, file_size, nullptr, GL_STREAM_READ);

        //unBind
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }
    void WindowControl::set_screenshot_size()
    {
        padding_ = (get_relative_width() * (4 - byte_per_pixel_)) % 4;
        stride_size_ = get_relative_width() * byte_per_pixel_ + padding_;
        file_size_ = stride_size_ * get_relative_height();
    }
    int WindowControl::get_file_size()
    {
        return file_size_;
    }
    void WindowControl::read_pixels(int x, int y, int width, int height)
    {
        glReadPixels(
            x, y,
            width, height,
            ((byte_per_pixel_ == 3) ? GL_BGR : GL_BGRA),
            GL_UNSIGNED_BYTE,
            0);
    }
    GLubyte *WindowControl::get_window_pixel(unsigned int pbo_id, int x, int y, int width, int height)
    {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_id);

        read_pixels(x, y, width, height);
        GLubyte *ret = (GLubyte *)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
#ifdef DEBUG_MODE
        if (!ret)
        {
            std::cout << "error : glmapbuffer - window_control.cpp \n";
        }
#endif
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
#ifdef DEBUG_MODE
        if (!ret)
        {
            std::cout << "error : glmapbuffer - window_control.cpp \n";
        }
#endif
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        return ret;
    }

    GLubyte **WindowControl::get_window_halfpixel()
    {
        GLubyte **pbomem = new GLubyte *[2];

        pbomem[0] = get_window_pixel(pbo_[0], 0, 0, relative_width_, relative_height_ / 2);

        pbomem[1] = get_window_pixel(pbo_[1], 0, relative_height_ / 2, relative_width_, relative_height_ / 2);
        return pbomem;
    }
    GLubyte **WindowControl::get_window_halfpixel(unsigned int pbo_id1, unsigned int pbo_id2)
    {
        GLubyte **pbomem = new GLubyte *[2];

        pbomem[0] = get_window_pixel(pbo_id1, 0, 0, relative_width_, relative_height_ / 2);

        pbomem[1] = get_window_pixel(pbo_id2, 0, relative_height_ / 2, relative_width_, relative_height_ / 2);

#ifdef DEBUG_MODE
        if (!pbomem[0])
        {
            std::cout << "error : glMapBuffer - window_control.cpp - pbo_[0]\n";
        }
        if (!pbomem[1])
        {
            std::cout << "error : glMapBuffer - window_control.cpp - pbo_[1]\n";
        }
#endif
        return pbomem;
    }
    GLubyte *WindowControl::get_window_fullpixel(unsigned int pbo_id)
    {
        return get_window_pixel(pbo_id, 0, 0, get_relative_width(), get_relative_height());
    }

}
/*
    for file process
*/
namespace nsg
{
    void WindowControl::screen_shot(const char *filename)
    {
        GLubyte *full_pixel = get_window_fullpixel(pbo_[2]);
        stbi_write_png(filename, get_relative_width(), get_relative_height(), byte_per_pixel_, full_pixel, stride_size_);
#ifdef DEBUG_MODE
        GLubyte **half_pixel = get_window_halfpixel(pbo_[0], pbo_[1]);
        stbi_write_png(std::string("1" + std::string(filename)).c_str(), get_relative_width(), get_relative_height() / 2, byte_per_pixel_, half_pixel[0], stride_size_);
        stbi_write_png(std::string("2" + std::string(filename)).c_str(), get_relative_width(), get_relative_height() / 2, byte_per_pixel_, half_pixel[1], stride_size_);
        delete[] half_pixel;
#endif
    }

    void WindowControl::byte_to_file(GLubyte **pbomem, const char *filename)
    {
        stbi_write_png(std::string("1" + std::string(filename)).c_str(), get_relative_width(), get_relative_height() / 2, byte_per_pixel_, pbomem[0], stride_size_);
        stbi_write_png(std::string("2" + std::string(filename)).c_str(), get_relative_width(), get_relative_height() / 2, byte_per_pixel_, pbomem[1], stride_size_);
    }
    void WindowControl::byte_to_file(GLubyte *pbomem, const char *filename, int width, int height)
    {

        stbi_write_png(filename, width, height, byte_per_pixel_, pbomem, stride_size_);
    }
    void WindowControl::byte_to_file(GLubyte *pbomem, const char *filename, int posy, int width, int height)
    {

        stbi_write_png(filename, width, height, byte_per_pixel_, pbomem + stride_size_ * posy, stride_size_);
    }
}