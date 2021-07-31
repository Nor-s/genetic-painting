#ifndef MY_BRUSH_H
#define MY_BRUSH_H

#include <vector>
#include "myheader/square_object.h"

namespace nsg {
    class Brush {
    private:
        static std::vector<SquareObject*> brushes_;
        static int brushes_size_;
        float translate_[3];
        float rotate_;
        float scale_[3];
        float brightness_;
        int idx_;
    public:
        Brush();
        static void init_brushes();
        static int get_brushes_size();
        
        void set_translate(float translate[3]);
        void set_scale(float scale[3]);
        void set_rotate(float rotate);
        void set_brightness(float brightness);
        void set_brushidx(int idx);
        void draw();
    };
}
#endif