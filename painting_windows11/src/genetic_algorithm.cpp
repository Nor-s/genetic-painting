#include "myheader/genetic_algorithm.h"

namespace nsg
{
    SquareObject *GeneticAlgorithm::picture_ = nullptr;
    Population *GeneticAlgorithm::population_ = nullptr;

    GeneticAlgorithm::GeneticAlgorithm(int population_size, int dna_len, int max_stage)
    {
        population_size_ = population_size;
        dna_len_ = dna_len;
        max_stage_ = max_stage;
    }
    void GeneticAlgorithm::loop_until_drop()
    {
        //for drop image
        while (WindowControl::size_ == 1 && !glfwWindowShouldClose(WindowControl::g_windows_[0]->get_window()))
        {
            process_input();
            glfwPollEvents();
        }
    }
    void GeneticAlgorithm::start_main_loop()
    {
        while (!glfwWindowShouldClose(WindowControl::g_windows_[0]->get_window()))
        {
            process_input();
            rendering_0();
            rendering_1();

            glfwPollEvents();
        }
    }
    void GeneticAlgorithm::init_population()
    {
        glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
        if (population_ == nullptr)
        {
            Brush::init_brushes();
            population_ = new Population(
                population_size_, dna_len_, max_stage_,
                0.0f, 0.0f,
                WindowControl::g_width_, WindowControl::g_height_,
                {0.2f, 0.4f});
            set_picture_to_data(picture_);
        }
    }

    void GeneticAlgorithm::process_input()
    {
        if (glfwGetKey(WindowControl::g_windows_[0]->get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(WindowControl::g_windows_[0]->get_window(), true);
        }
        else if (WindowControl::size_ == 2 && glfwGetKey(WindowControl::g_windows_[1]->get_window(), GLFW_KEY_1) == GLFW_PRESS)
        {
            WindowControl::rendering_lock();

            glDrawBuffer(GL_BACK);
            WindowControl::g_windows_[1]->window_clear_white();
            rendering_1();
            WindowControl::g_windows_[1]->window_capture("sdf");

            WindowControl::rendering_unlock();
        }
    }

    void GeneticAlgorithm::rendering_1()
    {
        if (WindowControl::size_ == 2)
        {
            WindowControl::rendering_lock();
            glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
            WindowControl::g_windows_[1]->window_clear_white();

            if (population_ != nullptr)
            {
                caculate_fitness();
                population_->sort_dna();
                WindowControl::g_windows_[1]->window_clear_white();
                population_->top()->draw_all();
#ifdef DEBUG_MODE
                std::cout << "top: " << population_->top()->fitness_ref() << "\n";
                int population_size = population_->get_population_size();
                for (int i = 0; i < population_size; i++)
                {
                    std::cout << i << " : " << population_->get_fitness(i) << "\n";
                }
                std::cout << "-----\n";
#endif
                population_->next_stage();
            }
            glfwSwapBuffers(WindowControl::g_windows_[1]->get_window());
            WindowControl::rendering_unlock();
        }
    }
    void GeneticAlgorithm::rendering_0()
    {
        if (WindowControl::size_ == 2)
        {
            WindowControl::rendering_lock();
            glfwMakeContextCurrent(WindowControl::g_windows_[0]->get_window());

            WindowControl::g_windows_[0]->window_clear_white();
            picture_->draw();
            glfwSwapBuffers(WindowControl::g_windows_[0]->get_window());

            WindowControl::rendering_unlock();
        }
    }

}

namespace nsg {
    GLubyte **GeneticAlgorithm::get_dna_byte(DNA* dna)
    {
        WindowControl::rendering_lock();
        glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
        glDrawBuffer(GL_BACK);
        WindowControl::g_windows_[1]->window_clear_white();
        dna->draw_all();
        GLubyte **ret = WindowControl::g_windows_[1]->get_window_pixel();
        WindowControl::rendering_unlock();
        return ret;
    }
    void GeneticAlgorithm::set_picture_to_data(SquareObject *picture)
    {
        WindowControl::rendering_lock();
        GLFWwindow *tmpWindow = glfwGetCurrentContext();

        glfwMakeContextCurrent(WindowControl::g_windows_[0]->get_window());
        glDrawBuffer(GL_BACK);
        WindowControl::g_windows_[0]->window_clear_white();
        picture->draw();
        grayscale_picture_ = WindowControl::g_windows_[1]->get_window_pixel();
#ifdef DEBUG_MODE
        WindowControl::g_windows_[1]->screenshot(grayscale_picture_);
#endif
        glfwMakeContextCurrent(tmpWindow);
        WindowControl::rendering_unlock();
    }
    void GeneticAlgorithm::caculate_fitness()
    {
        for (int i = 0; i < population_size_; i++)
        {
            population_->fitness_ref(i) = fitnessFunction(
                grayscale_picture_,
                get_dna_byte(population_->get_dna(i)),
                WindowControl::g_width_,
                WindowControl::g_height_);
        }
    }
}
/*
    using cosine similarity:  https://en.wikipedia.org/wiki/Cosine_similarity
*/
double fitnessFunction(GLubyte **a, GLubyte **b, int width, int height)
{
    double ret = 0.0;
    double dot = 0.0, denomA = 0.0, denomB = 0.0;
    for (int s = 0; s < 1; s++)
    {
        for (int i = 0; i < height / 2; i++)
        {
            for (int j = 0; j < width * 3; j += 3)
            {
                int k = i * width * 3 + j;
                dot += a[s][k] * b[s][k];
                denomA += a[s][k] * a[s][k];
                denomB += b[s][k] * b[s][k];
            }
        }
        ret = (dot / (sqrt(denomA) * sqrt(denomB)));
        denomA = dot = denomB = 0.0;
    }
    delete[] b;
    return ret;
}