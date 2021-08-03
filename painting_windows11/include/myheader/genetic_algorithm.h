#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <thread>
#include <mutex>

#include "myheader/population.h"
#include "myheader/window_control.h"
#include "myheader/picture.h"

namespace nsg
{
    class GeneticAlgorithm
    {
    private:
        std::pair<float, float> brush_width_;
        int population_size_;
        int dna_len_;
        int max_stage_;
        int population_num_;

        unsigned int target_pbo_full_;
        unsigned int* population_pbo_;
        double* best_score_;

        GLubyte *target_picture_full_;
        Picture *current_top_painting_;

    public:
        static int picture_byte_per_pixel_;
        static GeneticAlgorithm *manager_;
        static Picture *picture_;
        Population **population_;
        GeneticAlgorithm(int population_size, int dna_len, int max_stage, int g_population_num, std::pair<float, float> brush_width);
        ~GeneticAlgorithm();

        void init_population_pbo(int idx, int height);
        void init_target_pbo_full(int size);

        void process_input();
        void rendering_0();
        void rendering_1();
        void loop_until_drop();
        void start_main_loop();
        void init_population();
        void caculate_fitness(Population* population, unsigned int pbo_id);
        void caculate_fitness(Population *population, unsigned int population_pbo, double *fitness);

        void start_algorithm(Population* pop, unsigned int pop_id);


        void set_picture_to_data(SquareObject *picture);
        void set_current_top_painting();
        GLubyte *get_dna_byte_full(DNA *dna, unsigned int pbo_id, int x, int y, int width, int height, bool draw_dna);
    };
}
double fitnessFunction(GLubyte *a, GLubyte *b, int posx, int posy, int width, int height, int channel, bool is_gray);
int getRelativeVal(int x);
#endif