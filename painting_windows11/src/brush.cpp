#include "myheader/brush.h"

namespace nsg {
    std::vector<SquareObject*> Brush::brushes_;
    int Brush::brushes_size_ = 0;
    Brush::Brush() {}
    void Brush::init_brushes() {
        if(Brush::brushes_size_ == 0) {
            for(int i = 0; i < 4; i++) {
                Brush::brushes_.push_back(new SquareObject(i));
                Brush::brushes_size_++;
            }  
        }
    }
    int Brush::get_brushes_size() {
        return brushes_size_;
    }
    void Brush::set_translate(float translate[3]) {
        for(int i = 0; i < 3; i++) {
            translate_[i] = translate[i];
        }
    }
    void Brush::set_scale(float scale[3]) {
        for(int i = 0; i < 3; i++) {
            scale_[i] = scale[i];
        }
    }
    void Brush::set_rotate(float rotate) {
        rotate_ = rotate;
    }
    void Brush::set_brightness(float brightness) {
        brightness_ = brightness;
    }
    void Brush::set_brushidx(int idx) {
        idx_ = idx;
    }
    void Brush::draw() {
        SquareObject *p = brushes_[idx_];
        p->init_model();
        p->translate(translate_);
        p->rotate(rotate_);
        p->scale(scale_);
        p->scale(0.4);
        p->set_bright_to_uniform(brightness_);
        p->set_model_to_uniform();
        p->draw();
    }
}