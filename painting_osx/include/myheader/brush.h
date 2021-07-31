#ifndef MY_BRUSH_H
#define MY_BRUSH_H

#include <vector>
#include "myheader/square_object.h"

namespace nsg {
    struct Brush {
        static std::vector<SquareObject*> brushes_;
        static int brushes_size_;
        static void init_brushes();
        static int get_brushes_size();

        float translate_[3];
        float rotate_;
        float scale_[3];
        float brightness_;
        int idx_;

        Brush();
        void set_translate(float translate[3]);
        void set_scale(float scale[3]);
        void set_rotate(float rotate);
        void set_brightness(float brightness);
        void set_brushidx(int idx);
        void draw();
    };
}
#endif