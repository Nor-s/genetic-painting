#include "myheader/genetic_algorithm.h"

namespace nsg
{
    SquareObject *GeneticAlgorithm::picture_ = nullptr;
    GA *GeneticAlgorithm::genetic_algo_ = nullptr;

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
        if (genetic_algo_ == nullptr)
        {
            Brush::init_brushes();
            genetic_algo_ = new GA(
                population_size_, dna_len_, max_stage_,
                0.0f, 0.0f,
                WindowControl::g_width_, WindowControl::g_height_,
                0.2f, 0.4f);
            genetic_algo_->set_picture_to_data(picture_);
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

            if (genetic_algo_ != nullptr)
            {
                genetic_algo_->caculateFitness();
                genetic_algo_->sortDNA();
                WindowControl::g_windows_[1]->window_clear_white();
                genetic_algo_->top()->drawAll();
#ifdef DEBUG_MODE
                std::cout << "top: " << genetic_algo_->top()->fitnessRef() << "\n";
                int genetic_algo_Size = genetic_algo_->size();
                for (int i = 0; i < genetic_algo_Size; i++)
                {
                    std::cout << i << " : " << genetic_algo_->getFitness(i) << "\n";
                }
                std::cout << "-----\n";
#endif
                genetic_algo_->nextGeneration();
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