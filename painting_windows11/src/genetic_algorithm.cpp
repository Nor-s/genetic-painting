#include "myheader/genetic_algorithm.h"
//#define DEBUG_MODE
namespace nsg
{
    GeneticAlgorithm *GeneticAlgorithm::manager_ = nullptr;
    Picture *GeneticAlgorithm::picture_ = nullptr;
    Population *GeneticAlgorithm::population_ = nullptr;
    int GeneticAlgorithm::picture_byte_per_pixel_;

    /*
        init Algorithm
            population size  : number of dna
            dna len          : number of chromosome
            max stage        : set maximum gen picture 
    */
    GeneticAlgorithm::GeneticAlgorithm(int population_size, int dna_len, int max_stage, std::pair<float, float> brush_width)
    {
        best_score_ = 0;
        population_size_ = population_size;
        dna_len_ = dna_len;
        max_stage_ = max_stage;
        brush_width_ = brush_width;
        manager_ = this;
    }
    /*
        loop until drop image file
            don't drop othre file
            current set : RGB image file
    */
    void GeneticAlgorithm::loop_until_drop()
    {
        //for drop image
        while (picture_ == nullptr && !glfwWindowShouldClose(WindowControl::g_windows_[0]->get_window()))
        {
            process_input();
            glfwPollEvents();
        }
    }
    /*
        main loop 
            process input : key 1 is screenshot each of windows
            rendering_0   : drawing dropped image picture
            rendering_1   : drawing current top picture
    */
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
    /*
        1) init current_top_painting 
            it is first "all black" 
            size: full window (global width x global height)
        
        2) init population
            set DNA's
            DNA = palette 
            palette is brush attribute: decide scale, pos, angle, bright, brush type

        3) init picture data
            for caculate fitness
    */
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
    /*
        input
            esc : terminate program
            click window1 and key '1' : screenshot window 1
            click window0 and key '1' : screenshot window 0
    */
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
    /*
        rendering second window
            window[1] : drawing current top picture
            offscreen : caculate fitness
    */
    void GeneticAlgorithm::rendering_1()
    {
        if (WindowControl::size_ == 2)
        {
            glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());

            if (population_ != nullptr)
            {
                for(int i = 0; i < max_stage_; i++) {
                    caculate_fitness();
                    population_->sort_dna();
                    population_->next_stage();
                }
#ifdef DEBUG_MODE
                std::cout << "top: " << population_->top()->fitness_ref() << "\n";
                int population_size = population_->get_population_size();
                for (int i = 0; i < population_size; i++)
                {
                    std::cout << i << " : " << population_->fitness_ref(i) << "\n";
                }
                std::cout << population_->get_current_stage() << "-----\n";
#endif
            if(best_score_ < population_->top()->fitness_ref()){
                best_score_ = population_->top()->fitness_ref();
                glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
                WindowControl::g_windows_[1]->clear_window_white();
                current_top_painting_->bind_write_pbo_pointer();
                current_top_painting_->draw();
                population_->top()->draw_all();
                current_top_painting_->read_back_buffer();
                current_top_painting_->unbind_write_pbo();
                current_top_painting_->sub_picture();
            }
            }
        }
    }
    /*
        rendering first window
            drawing picture (dorpped image).
    */
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
    /*
        dna + current top painting
            to byte
            for fitness.
    */
    GLubyte **GeneticAlgorithm::get_dna_byte(DNA *dna)
    {
        glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
        glDrawBuffer(GL_BACK);
        WindowControl::g_windows_[1]->clear_window_white();
        current_top_painting_->draw();
        dna->draw_all();
        GLubyte **ret = WindowControl::g_windows_[1]->get_window_halfpixel();
#ifdef DEBUG_MODE
        //WindowControl::g_windows_[1]->byte_to_file(ret, "dna.png");
#endif
        return ret;
    }
    /*
        set grayscale_picture 
            using window_control' get window halfpixel
            for fitness.
    */
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
    /*
        set all DNA' fitness
            fitness : similarity of (dna + current top painting) and (grayscale picture)  
            for dna sort, get top.
    */
    void GeneticAlgorithm::caculate_fitness()
    {
        int size = population_->get_population_size();
        for (int i = 0; i < size; i++)
        {
            population_->fitness_ref(i) = fitnessFunction(
                grayscale_picture_,
                get_dna_byte(population_->get_dna(i)),
                WindowControl::g_windows_[1]->get_relative_width(),
                WindowControl::g_windows_[1]->get_relative_height());
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

    for (int i = 0; i < height / 2; i++)
    {
        for (int j = 0; j < width * 3; j += 3)
        {
            int k = i * width * 3 + j;
            dot += a[0][k] * b[0][k] + a[1][k] * b[1][k];
            denomA += a[0][k] * a[0][k]+a[1][k] * a[1][k];
            denomB += b[0][k] * b[0][k]+b[1][k] * b[1][k];
        }
    }
    ret = (dot / (sqrt(denomA) * sqrt(denomB)));
    delete[] b;
    return ret;
}