#include "myheader/dna_brush.h"
/*

    Dna
    
*/
namespace genetic_painting {
    Dna::Dna(const Dna& a){
        dna_len_ = a.dna_len_;
        x_ = a.x_;
        y_ = a.y_;
        width_ = a.width_;
        height_ = a.height_;
        sx_ = a.sx_;
        sxx_ = a.sxx_;
        for(int i = 0; i < dna_len_; i++) {
            this->dna_.push_back(a.dna_[i]);
        }
        fitness_ = a.fitness_;
    }
    Dna::Dna(const Dna& a, const Dna& b) {
        int pivot = rand()%(dna_len_-1) + 1;
        for(int i = 0; i < pivot; i++) {
            dna_.push_back(a.dna_[i]);
        }
        for(int i = pivot; i < dna_len_; i++) {
            dna_.push_back(b.dna_[i]);
        }
        fitness_ = 0.0f;
    }
    Dna::Dna(int dna_len,  float x, float y, float width, int height, float sx, float sxx) {
        dna_len_ = dna_len;
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;
        sx_ = sx;
        sxx_ = sxx;
        initDna();
    }    
    Dna::~Dna() {

    }
    bool Dna::operator <(Dna& a) {
        return fitness_ < a.fitness_;
    }    
    void Dna::initDna() {
        dna_ = std::vector<Brush>(dna_len_);
        for(int i = 0; i < dna_len_; i++) {
            set_translate_rand(i);
            set_brightness_rand(i);
            set_scale_rand(i);
            set_brush_randidx(i);
            set_rotate_rand(i);
        }
    }
    void Dna::self_mutate() {
        for(int i = 0; i < dna_len_; i++) {
            if(rand()%10 == 0) {
                if(rand()%2 == 0) {
                    set_translate_rand(i);
                }
                if(rand()%2 == 0) {
                    set_rotate_rand(i);
                }
                if(rand()%2 == 0) {
                    set_scale_rand(i);
                }
                if(rand()%2 == 0) {
                    set_brightness_rand(i);
                }
                if(rand()%2 == 0) {
                    set_brush_randidx(i);
                }
            }
        }
    }
    Dna* Dna::crossover(Dna& a) {
        return new Dna(*this, a);
    } 
    void Dna::draw(int i, glm::mat4& projection) {
        dna_[i].draw(projection);
    }

    void Dna::draw_all(glm::mat4& projection) {
        for(int i = 0; i < dna_len_; i++) {
            draw(i, projection);
        }
    }
    int Dna::size() {
        return dna_len_;
    }

    float& Dna::fitnessRef() {
        return fitness_;
    }
    float Dna::get_rand_float(float lo, float hi) {
        return lo + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(hi-lo)));
    }
    void Dna::set_translate_rand(int idx) {
        float randx = get_rand_float(-width_/2.0f + x_, width_/2.0f+ x_);
        float randy = get_rand_float(-height_/2.0f + y_, height_/2.0f + y_);
        float randz = get_rand_float(2.0f, 50.0f);
        dna_[idx].set_translate(randx, randy, 0.0f - randz);
    }
    void Dna::set_scale_rand(int idx) {
        float randsx = get_rand_float(sx_, sxx_);
        dna_[idx].set_scale(randsx);
    }
    void Dna::set_rotate_rand(int idx) {
        float randangle = get_rand_float(0.0f, 360.0f);
        dna_[idx].set_rotate(randangle);
    }
    void Dna::set_brightness_rand(int idx) {
        float randbrightness = get_rand_float(0.0f, 1.0f);
        dna_[idx].set_brightness(randbrightness);
    }
    void Dna::set_brush_randidx(int idx) {
        dna_[idx].set_brush_randidx();
    }
}
