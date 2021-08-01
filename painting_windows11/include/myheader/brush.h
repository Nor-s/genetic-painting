#ifndef MY_BRUSH_H
#define MY_BRUSH_H

#include <vector>
#include "myheader/square_object.h"

namespace nsg {
    class Brush : public SquareObject {
    private:
        const char *tex_[4] = {
            "../resources/1.png",
            "../resources/4.png",
            "../resources/6.png",
            "../resources/9.png",
        };
    public:
        Brush(int idx);
        
    };
    struct Palette {
        static std::vector<Brush*> brushes_;
        static int brushes_size_;
        static void init_brushes();
        static int get_brushes_size();

        float translate_[3];
        float rotate_;
        float scale_[3];
        float brightness_;
        int idx_;

        Palette();
        void set_translate(float translate[3]);
        void set_scale(float scale[3]);
        void set_rotate(float rotate);
        void set_brightness(float brightness);
        void set_brushidx(int idx);
        void draw();
    };
}
#endif