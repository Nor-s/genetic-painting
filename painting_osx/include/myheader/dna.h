#ifndef MY_GA_H
#define MY_GA_H

#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <stdlib.h>

#include "myheader/brush.h"
/*
            crossover     : child just point their chromosome, one crossover
            mutation      : reset pos, size, angle, brush type, brightness (current possiblity = 1%)
            draw          : using PBO, draw back or front buffer 
            getPicture    : buffer data => GLubyte  : (0~ height/2, heihgt/2 ~ height)           
            similarity    : GA class measures similarity using getPicture. and return to DNA for sorting
*/
namespace nsg
{

    class DNA
    {
    private:
        std::vector<Palette> dna;
        double fitness_;
        std::pair<float, float> brush_width_;
        int dna_len_;
        float x_;
        float y_;
        float width_;
        float height_;

    public:
        DNA(DNA &a, DNA &b, std::pair<float, float> &brush_width);
        DNA(int n, float x, float y, float width, float height, std::pair<float, float> brush_width_);
        ~DNA();
        bool operator<(DNA &a);
        double &fitness_ref();
        void initDNA();
        void mutate();
        DNA *crossover(DNA &a, std::pair<float, float> &brush_width);
        void draw(int i);
        void draw_all();
        void set_translate_relative(Palette &brush, std::pair<float, float> range_x, std::pair<float, float> range_y);
        void set_translate(Palette &brush);
        void set_scale(Palette &brush);
        void set_rotate(Palette &brush);
        void set_color(Palette &brush);
        void set_brushidx(Palette &brush);
        int size();
    };
    float getRandFloat(float lo, float hi);
}
#endif