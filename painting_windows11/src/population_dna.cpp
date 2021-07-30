#include "myheader/population_dna.h"
/*

    population_

*/
namespace genetic_painting {
    Population::Population(int popSize, int DnaSize, int maxGen, float x, float y, float width, float height, float sx, float sxx) {
        if(Brush::brushes_.size() == 0) {
            Brush::init_brushes();
        }

        population_size_ = popSize;
        dna_len_ = DnaSize;
        current_stage_ = 0;
        max_stage_ = maxGen;
        origin_image_height_ = static_cast<int>(height);
        origin_image_width_ = static_cast<int>(width);

        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;
        sx_ = sx;
        sxx_ = sxx;
        init_population();
    }
    void Population::init_population() {
        for(int i = 0; i < population_size_; i++) {
            population_.push_back(
                new Dna(dna_len_,x_, y_,width_, height_,sx_, sxx_ )
            );
        }
    }
    void Population::push_back(Dna* a) {
        population_.push_back(a);
        population_size_++;
    }
    void Population::pop_back(){
        Dna* tmp = population_.back();
        population_.pop_back();
        delete tmp;
        population_size_--;
    }
    void Population::draw_dna(int idx, glm::mat4& projection) {
        population_[idx]->draw_all(projection);
    }
    Dna* Population::get_top_dna(){
        return population_[0];
    }
    int Population::get_population_size() {
        return population_size_;
    }
    float Population::get_dna_fitness(int idx) {
        return population_[idx]->fitnessRef();
    }
    void Population::set_dna_fitness(int idx, float fitness) {
        population_[idx]->fitnessRef() = fitness;
    }

    void Population::sort_population() {
        std::sort(population_.begin(), population_.end(), compGreater);
    }
    void Population::next_stage() {
        int size = population_size_;
        for(int i = 0; i < size/2; i++) {
            pop_back();
        }
        for(int i = 0; i < size/2; i++) {
            int idx = rand()%population_size_;
            int jdx = rand()%population_size_;
            Dna* tmp = population_[idx]->crossover(*population_[jdx]);
            tmp->self_mutate();
            push_back(tmp);
        }
    }
    bool compLess(Dna* a, Dna* b) {
        return a->fitnessRef() < b->fitnessRef();
    }
    bool compGreater(Dna* a, Dna* b) {
        return a->fitnessRef() > b->fitnessRef();
    }
}    
