#include "myheader/dna.h"
//#define DEBUG_MODE
namespace nsg
{

    DNA::DNA(DNA &a, DNA &b, std::pair<float, float> &brush_width)
    {
        dna_len_ = a.dna_len_;
        x_ = a.x_;
        y_ = a.y_;
        width_ = a.width_;
        height_ = a.height_;
        brush_width_ = brush_width;
        for (int i = 0; i < dna_len_; i++)
        {
            if (rand() % 2 == 0)
            {
                dna.push_back(a.dna[i]);
            }
            else
            {
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
            set_color(dna.back());
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
    void DNA::set_color(Palette &brush)
    {
        float color[4] = {
            getRandFloat(0.0f, 1.0f),
            getRandFloat(0.0f, 1.0f),
            getRandFloat(0.0f, 1.0f),
            getRandFloat(0.0f, 0.99f)};
        if (SquareObject::g_is_grayscale_ == true)
        {
            color[0] = color[1] = color[2] = 0.2126 * color[0] + 0.7152 * color[1] + 0.0722 * color[2];
        }
        brush.set_color(color);
    }
    void DNA::set_brushidx(Palette &brush)
    {
        brush.set_brushidx(rand() % Palette::get_brushes_size());
    }
    void DNA::set_scale(Palette &brush)
    {
        float a = getRandFloat(brush_width_.first, brush_width_.second);
        float sxyz[3] = {a, a, 1.0};
        brush.set_scale(sxyz);
    }
    void DNA::set_translate_relative(Palette &brush, std::pair<float, float> range_x, std::pair<float, float> range_y)
    {
        float &current_x = brush.translate_[0];
        float &current_y = brush.translate_[1];
        float newpos[2];
        do
        {
            newpos[0] = getRandFloat(range_x.first, range_x.second);
            newpos[1] = getRandFloat(range_y.first, range_y.second);
        } while (current_x + newpos[0] > width_ +x_|| current_x + newpos[0] < x_ ||
                 current_y + newpos[1] > height_ + y_|| current_y + newpos[1] < y_);
        current_x += newpos[0];
        current_y += newpos[1];
    }
    void DNA::set_translate(Palette &brush)
    {
        float txyz[3] = {
            getRandFloat(x_, width_ + x_),
            getRandFloat(y_, height_  + y_),
            -getRandFloat(2.0f, 50.0f)};
        brush.set_translate(txyz);
#ifdef DEBUG_MODE
    std::cout<<y_<<" h: "<<height_<<"\n";
    std::cout<<txyz[0]<<" "<<txyz[1]<<" "<<txyz[2]<<"\n";
#endif
    }
    void DNA::mutate()
    {
        for (int i = 0; i < dna_len_; i++)
        {
            set_brushidx(dna[i]);
            if(rand()%2==0) 
            {
                set_rotate(dna[i]);
            }
            if (rand() % 5 == 0)
            {
                if (rand() % 2 == 0)
                {
                    set_color(dna[i]);
                }
                if (rand() % 2 == 0)
                {
                    set_scale(dna[i]);
                }
                if (rand() % 6 == 0)
                {
                    set_translate(dna[i]);
                }
            }
            else {
                set_translate_relative(dna[i], {-5, 5}, {-5, 5});
            }
        }
    }
    DNA *DNA::crossover(DNA &a, std::pair<float, float> &brush_width)
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