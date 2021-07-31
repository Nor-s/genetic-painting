#include "myheader/window_control.h"

namespace nsg
{
    std::vector<WindowControl *> WindowControl::g_windows_;
    int WindowControl::size_ = 0;
    bool WindowControl::rendering_semaphore_ = true;
    int WindowControl::g_height_ = 800;
    int WindowControl::g_width_ = 800;

    WindowControl::WindowControl(int width, int height, const char *title)
    {
        stbi_flip_vertically_on_write(true);
        title_ = std::string(title);
        width_ = g_width_ = width;
        height_ = g_height_ = height;
        window_ = init_window();
        init_pbo();
        size_++;
    }
    WindowControl::~WindowControl() {
        glDeleteBuffers(3, pbo_);
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
        GLFWwindow *window = glfwCreateWindow(width_, height_, title_.c_str(), NULL, NULL);
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
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return window;
    }
    GLFWwindow *WindowControl::get_window()
    {
        return window_;
    }
    
    void WindowControl::rendering_lock()
    {
        while (!rendering_semaphore_)
        {
        }
        rendering_semaphore_ = false;
    }
    void WindowControl::rendering_unlock()
    {
        rendering_semaphore_ = true;
    }
    
    void WindowControl::init_pbo()
    {
        //pbo gen
        glGenBuffers(3, pbo_);
        set_buffersize_pbo();
    }
    void WindowControl::set_buffersize_pbo() 
    {
        unsigned int halfbuffer_size = 3 * width_ * height_ / 2;
        unsigned int fullbuffer_size = 3 * width_ * height_;

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[0]);
        glBufferData(GL_PIXEL_PACK_BUFFER, halfbuffer_size, nullptr, GL_DYNAMIC_READ);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[1]);
        glBufferData(GL_PIXEL_PACK_BUFFER, halfbuffer_size, nullptr, GL_DYNAMIC_READ);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[2]);
        glBufferData(GL_PIXEL_PACK_BUFFER, fullbuffer_size, nullptr, GL_DYNAMIC_READ);
        //unBind
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }

    void WindowControl::clear_window_white()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }
    void WindowControl::resize_window(int width, int height)
    {
        g_width_ = width_ = width;
        g_height_ = height_ = height;
        glViewport(0, 0, width, height);
        glfwSetWindowSize(window_, width, height);
        set_buffersize_pbo();
    }
    GLubyte **WindowControl::get_window_fullpixel()
    {
        GLubyte** ret = new GLubyte*[1];
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[2]);
        glReadPixels(
            0, 0,
            width_, height_,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            0);
        ret[0] = (GLubyte *)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        return ret;
    }
    GLubyte **WindowControl::get_window_halfpixel()
    {
        GLubyte **pbomem = new GLubyte *[2];

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[0]);
        glReadPixels(
            0, 0,
            width_, height_ / 2,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            0);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[1]);
        glReadPixels(
            0, height_ / 2,
            width_, height_ / 2,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            0);

        // Process partial images.  Mapping the buffer waits for
        // outstanding DMA transfers into the buffer to finish.
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[0]);
        pbomem[0] = (GLubyte *)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[1]);
        pbomem[1] = (GLubyte *)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[0]);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[1]);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

        return pbomem;
    }

    void WindowControl::window_to_file(const char *filename)
    {
        int stridesize = g_width_ * 3 + g_width_ % 4;

        GLubyte** tmp = get_window_halfpixel();
        stbi_write_png(std::string("1" + std::string(filename)).c_str(), width_, height_/2, 3, tmp[0], stridesize);
        stbi_write_png(std::string("2" + std::string(filename)).c_str(), width_, height_/2, 3, tmp[1], stridesize);
        delete[] tmp;      
      GLubyte** tmpp = get_window_fullpixel();
        stbi_write_png(filename, g_width_, g_height_, 3, tmpp[0], stridesize);
        delete[] tmpp;
    }

    void WindowControl::byte_to_file(GLubyte **pbomem, const char *filename)
    {
        int stridesize = g_width_ * 3 + g_width_ % 4;

        GLubyte *full = new GLubyte[width_ * height_ * 3];
        int size = stridesize * height_;
        for (int i = 0; i < size / 2; i++)
        {
            full[i] = pbomem[0][i];
            full[i + size / 2] = pbomem[1][i];
        }

        stbi_write_png(std::string("1" + std::string(filename)).c_str(), width_, height_/2, 3, pbomem[0], stridesize);
        stbi_write_png(std::string("2" + std::string(filename)).c_str(), width_, height_/2, 3, pbomem[1], stridesize);
        stbi_write_png(filename, width_, height_, 3, full, stridesize);
    }
}
