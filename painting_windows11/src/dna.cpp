#include "myheader/dna.h"

namespace nsg
{

    DNA::DNA(DNA &a, DNA &b, std::pair<float, float>& brush_width)
    {
        dna_len_ = a.dna_len_;
        x_ = a.x_;
        y_ = a.y_;
        width_ = a.width_;
        height_ = a.height_;
        brush_width_ = brush_width;
        for (int i = 0; i < dna_len_; i++)
        {
            if(rand()%2 == 0) {
                dna.push_back(a.dna[i]);
            }
            else {
                dna.push_back(b.dna[i]);
            }
            set_scale(dna.back());
        }
        fitness_ = 0;

    }

    DNA::DNA(int n, float x, float y, float width, float height, std::pair<float, float> brush_width)
    {
        dna_len_ = n;
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;
        brush_width_ = brush_width;
        initDNA();
    }
    DNA::~DNA()
    {
    }
    void DNA::initDNA()
    {
        for (int i = 0; i < dna_len_; i++)
        {
            dna.push_back(Palette());
            set_rotate(dna.back());
            set_brightness(dna.back());
            set_brushidx(dna.back());
            set_scale(dna.back());
            set_translate(dna.back());
        }
    }

    bool DNA::operator<(DNA &a)
    {
        return fitness_ < a.fitness_;
    }
    void DNA::set_rotate(Palette &brush)
    {
        brush.set_rotate(getRandFloat(0.0f, 360.0f));
    }
    void DNA::set_brightness(Palette &brush)
    {
        brush.set_brightness(getRandFloat(0.0f, 1.0f));
    }
    void DNA::set_brushidx(Palette &brush)
    {
        brush.set_brushidx(rand() % Palette::get_brushes_size());
    }
    void DNA::set_scale(Palette &brush)
    {
            float a = getRandFloat(brush_width_.first, brush_width_.second);
        float sxyz[3] = { a, a, 1.0};
      //  sxyz[0] = getRandFloat(brush_width_.first, brush_width_.second);
       // sxyz[1] = getRandFloat(brush_width_.first, brush_width_.second);
        brush.set_scale(sxyz);
    }
    void DNA::set_translate(Palette &brush)
    {
        float txyz[3] = {
            getRandFloat(-width_ / 2.0f + x_, width_ / 2.0f + x_),
            getRandFloat(-height_ / 2.0f + y_, height_ / 2.0f + y_),
            -getRandFloat(2.0f, 50.0f)};
        brush.set_translate(txyz);
    }
    void DNA::mutate()
    {
        for (int i = 0; i < dna_len_; i++)
        {
            if (rand() % 2 == 0)
            {
                if(rand()%2 == 0) {
                    set_rotate(dna[i]);
                }
                if(rand()%2 == 0) {
                    set_brushidx(dna[i]);
                }
                if(rand()%2 == 0) {
                    set_scale(dna[i]);
                }
                if (rand() % 2 == 0)
                {
                    set_brightness(dna[i]);
                }
                if (rand() % 10 == 0)
                {
                    set_translate(dna[i]);
                }
            }
        }
    }
    DNA *DNA::crossover(DNA &a, std::pair<float, float>& brush_width)
    {
        return new DNA(*this, a, brush_width);
    }
    void DNA::draw(int i)
    {
        dna[i].draw();
    }
    void DNA::draw_all()
    {
        for (int i = 0; i < dna_len_; i++)
        {
            this->draw(i);
        }
    }

    int DNA::size()
    {
        return dna_len_;
    }

    double &DNA::fitness_ref()
    {
        return fitness_;
    }

}
namespace nsg
{
    float getRandFloat(float lo, float hi)
    {
        return lo + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (hi - lo)));
    }
}