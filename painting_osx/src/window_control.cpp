#include "myheader/window_control.h"

namespace nsg
{
    myPainting *WindowControl::picture_ = nullptr;
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
        Shader::projection_matrix = glm::ortho(
            (float)-width / 2.0f, (float)width / 2.0f,
            (float)-height / 2.0f, (float)height / 2.0f,
            0.0f, 100.0f);
        init_pbo();
        size_++;
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

        return window;
    }
    GLFWwindow *WindowControl::get_window()
    {
        return window_;
    }
    void WindowControl::rendering_lock()
    {
#ifdef SEMAPHORE_TEST
        while (!rendering_semaphore_)
        {
        }
#endif
        rendering_semaphore_ = false;
    }
    void WindowControl::rendering_unlock()
    {
        rendering_semaphore_ = true;
    }
    void WindowControl::init_pbo()
    {
        glfwGetWindowSize(get_window(), &width_, &height_);
        unsigned int bufferSize = width_ * (height_ / 2) * 3;

        //pbo gen
        glGenBuffers(2, pbo_);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[0]);
        glBufferData(GL_PIXEL_PACK_BUFFER, bufferSize, nullptr, GL_STREAM_READ);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_[1]);
        glBufferData(GL_PIXEL_PACK_BUFFER, bufferSize, nullptr, GL_STREAM_READ);

        //unBind
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }

    void WindowControl::window_clear_white()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    GLubyte **WindowControl::get_window_pixel()
    {
        GLubyte **pbomem = new GLubyte *[2];

        init_pbo();

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

        glDeleteBuffers(2, pbo_);
        return pbomem;
    }

    void WindowControl::window_capture(const char *filename)
    {
        int strideSize = g_width_ * 3 + g_width_ % 4;

        GLubyte **pbomem = get_window_pixel();
        GLubyte *full = new GLubyte[width_ * height_ * 3];
        int size = strideSize * height_;
        for (int i = 0; i < size / 2; i++)
        {
            full[i] = pbomem[0][i];
            full[i + size / 2] = pbomem[1][i];
        }

        stbi_write_png("s11.png", width_, height_ / 2, 3, pbomem[0], strideSize);
        stbi_write_png("s111.png", width_, height_ / 2, 3, pbomem[1], strideSize);
        stbi_write_png(filename, width_, height_, 3, full, strideSize);

        delete[] full;
        delete[] pbomem;
        //delete
    }

    void WindowControl::screenshot(GLubyte **pbomem)
    {
        int stridesize = g_width_ * 3 + g_width_ % 4;

        GLubyte *full = new GLubyte[width_ * height_ * 3];
        int size = stridesize * height_;
        for (int i = 0; i < size / 2; i++)
        {
            full[i] = pbomem[0][i];
            full[i + size / 2] = pbomem[1][i];
        }

        stbi_write_png("s11.png", width_, height_ / 2, 3, pbomem[0], stridesize);
        stbi_write_png("s111.png", width_, height_ / 2, 3, pbomem[1], stridesize);
        stbi_write_png("s1111.png", width_, height_, 3, full, stridesize);
    }
}
