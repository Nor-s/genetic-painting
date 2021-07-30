#include "myheader/callback.h"

namespace nsg
{
    void init_callback(GLFWwindow *window)
    {
        //callback
        glfwSetDropCallback(window, drop_callback);
        glfwSetFramebufferSizeCallback(window, resize_callback);
        glfwSetWindowPosCallback(window, pos_callback);
    }

    void pos_callback(GLFWwindow *window, int x, int y)
    {
        // for adjacent two window
        int posx = 0, posy = 0;
        glfwGetWindowPos(WindowControl::g_windows_[0]->get_window(), &posx, &posy);
        if (WindowControl::size_ == 2)
        {
            glfwSetWindowPos(WindowControl::g_windows_[1]->get_window(), posx + WindowControl::g_windows_[0]->g_width_, posy);
        }
    }
    void resize_callback(GLFWwindow *windowPointer, int width, int height)
    {
        glViewport(0, 0, width, height);
        float widthFactor = static_cast<float>(width) / static_cast<float>(WindowControl::g_width_);
        float heightFactor = static_cast<float>(height) / static_cast<float>(WindowControl::g_height_);
        Shader::projection_matrix = glm::ortho(
            -(float)WindowControl::g_width_ / 2.0f * widthFactor, (float)WindowControl::g_width_ / 2.0f * widthFactor,
            -(float)WindowControl::g_height_ / 2.0f * heightFactor, (float)WindowControl::g_height_ / 2.0f * heightFactor,
            0.0f, 100.0f);
    }
    void drop_callback(GLFWwindow *window, int count, const char **paths)
    {
        if (WindowControl::size_ == 2)
        {
            return;
        }
        int i;
        for (i = 0; i < 1; i++)
        {
            WindowControl::rendering_lock();
#ifdef DEBUG_MODE
            std::cout << paths[i] << "\n";
#endif
            GeneticAlgorithm::picture_ = new myPainting(paths[i]);
            int posx, posy;
            int width = GeneticAlgorithm::picture_->texWidth, height = GeneticAlgorithm::picture_->texHeight;

            glfwSetWindowSize(WindowControl::g_windows_[0]->get_window(), width, height);
            glfwGetWindowPos(WindowControl::g_windows_[0]->get_window(), &posx, &posy);

            WindowControl::g_windows_.push_back(new WindowControl(width, height, "painting"));
            glfwSetWindowPos(WindowControl::g_windows_[1]->get_window(), posx + width, posy);
            init_callback(WindowControl::g_windows_[1]->get_window());

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            WindowControl::rendering_unlock();
        }
    }
}