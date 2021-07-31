#include "myheader/callback.h"

namespace nsg
{
    void render_callback()
    { 
    /*
        if (WindowControl::size_ == 2)
        {
            GeneticAlgorithm::manager_->rendering_0();
            glfwSwapBuffers(WindowControl::g_windows_[0]->get_window());
            GeneticAlgorithm::manager_->rendering_1();
            glfwSwapBuffers(WindowControl::g_windows_[1]->get_window());
        }
    */
    }
    void init_callback(GLFWwindow *window, bool is_drop)
    {
        //callback
        if (is_drop)
        {
            glfwSetDropCallback(window, drop_callback);
        }
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
        render_callback();
    }

    void resize_callback(GLFWwindow *window, int width, int height)
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
        for (int i = 0; i < 1; i++)
        {
#ifdef DEBUG_MODE
            std::cout << paths[i] << "\n";
#endif
            GeneticAlgorithm::picture_ = new SquareObject(paths[i]);
            int posx, posy;
            int width = GeneticAlgorithm::picture_->get_tex_width(), height = GeneticAlgorithm::picture_->get_tex_height();
            std::vector<WindowControl *> *window_p = &WindowControl::g_windows_;
            (*window_p)[0]->resize_window(width, height);
            glfwGetWindowPos((*window_p)[0]->get_window(), &posx, &posy);
            Shader::projection_matrix = glm::ortho(
                -(float)WindowControl::g_width_ / 2.0f, (float)WindowControl::g_width_ / 2.0f,
                -(float)WindowControl::g_height_ / 2.0f, (float)WindowControl::g_height_ / 2.0f,
                0.0f, 100.0f);

            (*window_p).push_back(new WindowControl(width, height, "painting"));
            glfwSetWindowPos((*window_p)[1]->get_window(), posx + width, posy);
            init_callback((*window_p)[1]->get_window(), false);

            render_callback();
        }
    }
}