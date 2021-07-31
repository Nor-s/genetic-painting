#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H


#include "myheader/population.h"
#include "myheader/window_control.h"
#include "myheader/square_object.h"

namespace nsg {
    class GeneticAlgorithm {
    private:
        int population_size_;
        int dna_len_;
        int max_stage_;

        GLubyte** grayscale_picture_;
    public:
        static Population* population_;
        static SquareObject *picture_;
        GeneticAlgorithm(int population_size, int dna_len, int max_stage);

        void process_input();
        void rendering_0();
        void rendering_1();
        void loop_until_drop();
        void start_main_loop();
        void init_population();
        void caculate_fitness();

        void set_picture_to_data(SquareObject *picture);
        GLubyte** get_dna_byte(DNA* dna);

    };
}
double fitnessFunction(GLubyte **a, GLubyte **b, int width, int height);
#endif