#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H


#include "myheader/population.h"
#include "myheader/window_control.h"
#include "myheader/picture.h"

namespace nsg {
    class GeneticAlgorithm {
    private:
        std::pair<float, float> brush_width_;
        int population_size_;
        int dna_len_;
        int max_stage_;

        GLubyte** grayscale_picture_;
        Picture *current_top_painting_;
    public:
        static GeneticAlgorithm *manager_;
        static Population* population_;
        static Picture *picture_;
        GeneticAlgorithm(int population_size, int dna_len, int max_stage, std::pair<float, float> brush_width);

        void process_input();
        void rendering_0();
        void rendering_1();
        void loop_until_drop();
        void start_main_loop();
        void init_population();
        void caculate_fitness();

        void set_picture_to_data(SquareObject *picture);
        void set_current_top_painting();
        GLubyte** get_dna_byte(DNA* dna);

    };
}
double fitnessFunction(GLubyte **a, GLubyte **b, int width, int height);
#endif