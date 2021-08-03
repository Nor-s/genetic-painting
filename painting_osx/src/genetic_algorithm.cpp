#include "myheader/genetic_algorithm.h"
//#define DEBUG_MODE
namespace nsg
{
    GeneticAlgorithm *GeneticAlgorithm::manager_ = nullptr;
    Picture *GeneticAlgorithm::picture_ = nullptr;
    int GeneticAlgorithm::picture_byte_per_pixel_;

    /*
        init Algorithm
            population size  : number of dna
            dna len          : number of chromosome
            max stage        : set maximum gen picture 
    */
    GeneticAlgorithm::GeneticAlgorithm(int population_size, int dna_len, int max_stage, int population_num, std::pair<float, float> brush_width)
    {
        population_ = nullptr;
        best_score_ = 0;
        population_size_ = population_size;
        dna_len_ = dna_len;
        max_stage_ = max_stage;
        brush_width_ = brush_width;
        manager_ = this;
        population_num_ = population_num;
    }
    GeneticAlgorithm::~GeneticAlgorithm()
    {
        for (int i = 0; i < population_num_; i++)
        {
            delete[] population_[i];
        }
        delete[] target_picture_; //To do : need unmap pbo for free pointer
        delete[] current_top_painting_;
        delete[] population_;
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
        population_ = new Population *[population_num_];
        Palette::init_brushes();
        init_population_pbo(WindowControl::get_file_size() / population_num_);
        init_target_pbo_full(WindowControl::get_file_size());

        int posy = 0;
        int div_height = WindowControl::g_height_ / population_num_;
        for (int i = 0; i < population_num_; i++)
        {
            population_[i] = new Population(
                population_size_, dna_len_, max_stage_,
                0, posy,
                WindowControl::g_width_, div_height,
                brush_width_);
            posy += div_height;
        }
        set_picture_to_data(picture_);
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
            WindowControl::g_windows_[1]->screen_shot("screenshot1.png");
            glReadBuffer(GL_BACK);
        }
        else if (WindowControl::size_ == 2 && glfwGetKey(WindowControl::g_windows_[0]->get_window(), GLFW_KEY_1) == GLFW_PRESS)
        {
            glfwMakeContextCurrent(WindowControl::g_windows_[0]->get_window());
            glReadBuffer(GL_FRONT);
            WindowControl::g_windows_[0]->screen_shot("screenshot2.png");
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
            double fitness = 0.0f;
            for(int i = 0; i < population_num_; i++) {
                start_algorithm(population_[i], population_pbo_[i]);
                fitness += population_[i]->top()->fitness_ref();
            }
        
            if (best_score_ < fitness)
            {
                best_score_ = fitness;
                WindowControl::g_windows_[1]->clear_window_white();
                current_top_painting_->prepare_sub_picture();
                current_top_painting_->draw();
                for(int i = 0; i < population_num_; i++) {
                    population_[i]->top()->draw_all();
                }
                current_top_painting_->start_sub_picture();
            }
        }
    }
    void GeneticAlgorithm::start_algorithm(Population *pop, unsigned int pop_id)
    {
        for (int i = 0; i < max_stage_; i++)
        {
            caculate_fitness(pop, pop_id);
            pop->sort_dna();
            pop->next_stage();
        }
#ifdef DEBUG_MODE
        std::cout << "top: " << pop->top()->fitness_ref() << "\n";
        int population_size = pop->get_population_size();
        for (int i = 0; i < population_size; i++)
        {
            std::cout << i << " : " << pop->fitness_ref(i) << "\n";
        }
        std::cout << pop[0]->get_current_stage() << "-----\n";
#endif
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
    GLubyte *GeneticAlgorithm::get_dna_byte_full(DNA *dna, unsigned int pbo_id, int x, int y, int width, int height)
    {
        glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
        glDrawBuffer(GL_BACK);
        WindowControl::g_windows_[1]->clear_window_white();
        current_top_painting_->draw();
        dna->draw_all();
        GLubyte *ret = WindowControl::g_windows_[1]->get_window_pixel(pbo_id, x, y, width, height);
#ifdef DEBUG_MODE
//WindowControl::g_windows_[1]->byte_to_file(ret, "painting.png", width, height);
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
        target_picture_ = WindowControl::g_windows_[0]->get_window_halfpixel();
        target_picture_full_ = WindowControl::g_windows_[0]->get_window_fullpixel(target_pbo_full_);
        WindowControl::g_windows_[0]->byte_to_file(target_picture_full_, "gray.png", WindowControl::g_width_, WindowControl::g_height_);
#ifdef DEBUG_MODE
        WindowControl::g_windows_[1]->byte_to_file(target_picture_, "gray.png");
#endif
        glfwMakeContextCurrent(tmpWindow);
    }
    /*
        set all DNA' fitness
            fitness : similarity of (dna + current top painting) and (grayscale picture)  
            for dna sort, get top.
    */
    void GeneticAlgorithm::caculate_fitness(Population *population, unsigned int population_pbo)
    {
        {
            int posx = population->get_posx(), posy = population->get_posy(), width = population->get_width(), height = population->get_height();
            int size = population->get_population_size();
#ifdef __APPLE__

#endif

#ifdef DEBUG_MODE
std::cout<<"posx : "<<posx<<" posy: "<<posy<<" width: "<<width<<" height: "<<height<<"\n"; 
WindowControl::g_windows_[0]->byte_to_file(target_picture_full_, "target.png", posy, width, height);
#endif

            for (int i = 0; i < size; i++)
            {
                population->fitness_ref(i) = fitnessFunction1(
                    target_picture_full_,
                    get_dna_byte_full(population->get_dna(i), population_pbo, posx, posy, width, height),
                    posx, posy,
                    width, height,
                    3,
                    SquareObject::g_is_grayscale_);
            }
        }
    }
}
/*
    using cosine similarity:  https://en.wikipedia.org/wiki/Cosine_similarity
*/
double fitnessFunction(GLubyte **a, GLubyte **b, int width, int height, int channel, bool is_gray)
{
    double ret = 0.0;
    double dot = 0.0, denomA = 0.0, denomB = 0.0;
    int adder = (is_gray) ? channel : 1;

    for (int i = 0; i < height / 2; i++)
    {
        for (int j = 0; j < width * channel; j += adder)
        {
            int k = i * width * channel + j;
            dot += a[1][k] * b[1][k] + a[0][k] * b[0][k];
            denomA += a[1][k] * a[1][k] + a[0][k] * a[0][k];
            denomB += b[1][k] * b[1][k] + b[0][k] * b[0][k];
        }
    }
    ret = (dot / (sqrt(denomA) * sqrt(denomB)));
    delete[] b;
    return ret;
}
double fitnessFunction1(GLubyte *a, GLubyte *b, int posx, int posy, int width, int height, int channel, bool is_gray)
{
    double ret = 0.0;
    double dot = 0.0, denomA = 0.0, denomB = 0.0;
    int adder = (is_gray) ? channel : 1;
    int y = posy;
    int x = posx * channel;
    int end_y = height + y;
    int line = width*channel + (width * (4- channel))%4;
    int end_x =line + x;


    for (int i = y; i < end_y; i++)
    {
        for (int j = x; j < end_x; j += adder)
        {
            int bk = (i - y) * line + (j - x);
            int ak = i * line + j;
            dot += a[ak] * b[bk];
            denomA += a[ak] * a[ak];
            denomB += b[bk] * b[bk];
        }
    }
    ret = (dot / (sqrt(denomA) * sqrt(denomB)));
    return ret;
}

namespace nsg
{
    void GeneticAlgorithm::init_population_pbo(int size)
    {
        population_pbo_ = new unsigned int[population_num_];
        glGenBuffers(population_num_, population_pbo_);
        for (int i = 0; i < population_num_; i++)
        {
            glBindBuffer(GL_PIXEL_PACK_BUFFER, population_pbo_[i]);
            glBufferData(GL_PIXEL_PACK_BUFFER, size, nullptr, GL_STREAM_READ);
        }
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }
    void GeneticAlgorithm::init_target_pbo_full(int size)
    {
        GLFWwindow *tmpWindow = glfwGetCurrentContext();

        glfwMakeContextCurrent(WindowControl::g_windows_[0]->get_window());
        glGenBuffers(1, &target_pbo_full_);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, target_pbo_full_);
        glBufferData(GL_PIXEL_PACK_BUFFER, size, nullptr, GL_STREAM_READ);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        glfwMakeContextCurrent(tmpWindow);
    }

}
int getRelativeVal(int x)
{
#ifdef __APPLE__
    x *= 2;
#endif
    return x;
}