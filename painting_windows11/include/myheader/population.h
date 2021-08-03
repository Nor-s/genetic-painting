#ifndef MY_DNA_H
#define MY_DNA_H

#include "myheader/dna.h"

namespace nsg
{
    class Population
    {
    private:
        std::vector<DNA *> population_;
        std::pair<float, float> current_brush_width_;
        int dna_len_;
        int population_size_;
        int max_stage_;
        int current_stage_;
        int width_;
        int height_;
        int posx_;
        int posy_;

    public:
        Population();
        Population(int populaiton_size, int dna_len, int max_stage, float x, float y, float width, int height, std::pair<float, float> current_brush_width);
        ~Population();
        double &fitness_ref(int idx);
        void init_population();
        void push_back(DNA *dna);
        void pop_back();
        void next_stage();
        void sort_dna();
        void draw_dna(int idx);
        int get_population_size();
        DNA *get_dna(int idx);
        DNA *top();

        void swap(int idx, DNA *a);

        int get_current_stage();
        int get_width();
        int get_height();
        int get_posx();
        int get_posy();
    };
}
#endif