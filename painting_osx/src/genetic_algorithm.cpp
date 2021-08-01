#include "myheader/genetic_algorithm.h"
#define DEBUG_MODE
namespace nsg
{
    GeneticAlgorithm *GeneticAlgorithm::manager_ = nullptr;
    Picture *GeneticAlgorithm::picture_ = nullptr;
    Population *GeneticAlgorithm::population_ = nullptr;

    GeneticAlgorithm::GeneticAlgorithm(int population_size, int dna_len, int max_stage, std::pair<float, float> brush_width)
    {
        population_size_ = population_size;
        dna_len_ = dna_len;
        max_stage_ = max_stage;
        brush_width_ = brush_width;
        manager_ = this;
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
            glfwSwapBuffers(WindowControl::g_windows_[0]->get_window());
            rendering_1();
            glfwSwapBuffers(WindowControl::g_windows_[1]->get_window());
            glfwPollEvents();
        }
    }
    void GeneticAlgorithm::init_population()
    {
        glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
        current_top_painting_ = new Picture(
            WindowControl::g_width_,
            WindowControl::g_height_,
            3,
            GL_BGR);
        if (population_ == nullptr)
        {
            Palette::init_brushes();
            population_ = new Population(
                population_size_, dna_len_, max_stage_,
                0.0f, 0.0f,
                WindowControl::g_width_, WindowControl::g_height_,
                brush_width_);
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
            glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
            glReadBuffer(GL_FRONT);
            WindowControl::g_windows_[1]->window_to_file("screenshot1.png");
            glReadBuffer(GL_BACK);
        }
        else if (WindowControl::size_ == 2 && glfwGetKey(WindowControl::g_windows_[0]->get_window(), GLFW_KEY_1) == GLFW_PRESS)
        {
            glfwMakeContextCurrent(WindowControl::g_windows_[0]->get_window());
            glReadBuffer(GL_FRONT);
            WindowControl::g_windows_[0]->window_to_file("screenshot2.png");
            glReadBuffer(GL_BACK);
        }
    }

    void GeneticAlgorithm::rendering_1()
    {
        if (WindowControl::size_ == 2)
        {
            glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
            WindowControl::g_windows_[1]->clear_window_white();

            if (population_ != nullptr)
            {
                for(int i = 0; i < max_stage_; i++) {
                    caculate_fitness();
                    population_->sort_dna();
                    population_->next_stage();
                }
                WindowControl::g_windows_[1]->clear_window_white();
                current_top_painting_->bind_write_pbo_pointer();
                current_top_painting_->draw();
                population_->top()->draw_all();
                current_top_painting_->read_back_buffer();
                current_top_painting_->unbind_write_pbo();
                current_top_painting_->sub_picture();
#ifdef DEBUG_MODE
                std::cout << "top: " << population_->top()->fitness_ref() << "\n";
                int population_size = population_->get_population_size();
                for (int i = 0; i < population_size; i++)
                {
                    std::cout << i << " : " << population_->fitness_ref(i) << "\n";
                }
                std::cout << population_->get_current_stage() << "-----\n";
#endif
            }
        }
    }
    void GeneticAlgorithm::rendering_0()
    {
        if (WindowControl::size_ == 2)
        {
            glfwMakeContextCurrent(WindowControl::g_windows_[0]->get_window());

            WindowControl::g_windows_[0]->clear_window_white();
            picture_->draw();
        }
    }

}

namespace nsg
{
    GLubyte **GeneticAlgorithm::get_dna_byte(DNA *dna)
    {
        glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
        glDrawBuffer(GL_BACK);
        WindowControl::g_windows_[1]->clear_window_white();
        current_top_painting_->draw();
        dna->draw_all();
        GLubyte **ret = WindowControl::g_windows_[1]->get_window_halfpixel();
        return ret;
    }
    void GeneticAlgorithm::set_picture_to_data(SquareObject *picture)
    {
        GLFWwindow *tmpWindow = glfwGetCurrentContext();

        glfwMakeContextCurrent(WindowControl::g_windows_[0]->get_window());
        glDrawBuffer(GL_BACK);
        WindowControl::g_windows_[0]->clear_window_white();
        picture->draw();
        grayscale_picture_ = WindowControl::g_windows_[1]->get_window_halfpixel();
#ifdef DEBUG_MODE
        WindowControl::g_windows_[1]->byte_to_file(grayscale_picture_, "gray.png");
#endif
        glfwMakeContextCurrent(tmpWindow);
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
    }
    ret = (dot / (sqrt(denomA) * sqrt(denomB)));
    delete[] b;
    return ret;
}