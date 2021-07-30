#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H


#include "myheader/myGA.h"
#include "myheader/window_control.h"
#include "myheader/myPainting.h"

namespace nsg {
    class GeneticAlgorithm {
        public:
        int population_size_;
        int dna_len_;
        int max_stage_;
        GeneticAlgorithm(int population_size, int dna_len, int max_stage);
        static myPainting *picture_;
        static GA* genetic_algo_;
        void process_input();
        void rendering_0();
        void rendering_1();
        void loop_until_drop();
        void start_main_loop();
        void init_population();

    };
}
#endif