#include "myheader/population.h"

namespace nsg
{
    Population::Population(int populaiton_size, int dna_len, int max_stage, float x, float y, float width, int height, std::pair<float, float> current_brush_width)
    {
        current_brush_width_ = current_brush_width;
        dna_len_ = dna_len;
        population_size_ = populaiton_size;
        max_stage_ = max_stage;
        current_stage_ = 0;
        width_ = width;
        height_ = height;

        init_population();
    }
    void Population::init_population()
    {
        int size = population_size_*2;
        for (int i = 0; i < size; i++)
        {
            population_.push_back(
                new DNA(
                    dna_len_,
                    0.0, 0.0,
                    width_, height_,
                    current_brush_width_));
        }
    }
    void Population::push_back(DNA *a)
    {
        population_.push_back(a);
    }
    void Population::pop_back()
    {
        if (population_.size() == 0)
        {
            return;
        }
        DNA *tmp = population_.back();
        population_.pop_back();
        delete tmp;
    }
    void Population::draw_dna(int idx)
    {
        population_[idx]->draw_all();
    }
    DNA *Population::top()
    {
        return population_[0];
    }

    int Population::get_population_size()
    {
        return population_.size();
    }
    DNA *Population::get_dna(int idx)
    {
        return population_[idx];
    }
    double &Population::fitness_ref(int idx)
    {
        return population_[idx]->fitness_ref();
    }

    void Population::sort_dna()
    {
        std::sort(
            population_.begin(),
            population_.end(),
            [](DNA *a, DNA *b) -> bool
            {
                return a->fitness_ref() > b->fitness_ref();
            });
    }
    void Population::swap(int idx, DNA* a) {
        DNA* tmp = population_[idx];
        population_[idx] = a;
        delete tmp;
    }
    void Population::next_stage()
    {
        int size = population_size_;

        current_stage_++;
        if(current_brush_width_.first > 0.03) {
            current_brush_width_.first -= 0.001;
            current_brush_width_.second -= 0.001;
        }
    
        for (int i = 0; i < size / 2; i++)
        {
            pop_back();
        }
        for (int i = 0; i < size / 2; i++)
        {
            int parent1 = rand() % 5;
            int parent2 = rand() % 5;
            while (parent1 == parent2)
            {
                parent1 = rand() % 5;
                parent2 = rand() % 5;
            }
            push_back(population_[parent1]->crossover(*population_[parent2],current_brush_width_));
            population_.back()->mutate();
        }
    }
    int Population::get_current_stage()
    {
        return current_stage_;
    }
}