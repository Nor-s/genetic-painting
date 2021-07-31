#include "myheader/dna.h"

namespace nsg
{
    DNA::DNA(DNA &a, DNA &b)
    {
        dnaSize = a.dnaSize;
        x_ = a.x_;
        y_ = a.y_;
        sx_ = a.sx_;
        width_ = a.width_;
        height_ = a.height_;
        sxx_ = a.sxx_;
        int pivot = rand() % (dnaSize - 2) + 1;
        for (int i = 0; i < pivot; i++)
        {
            dna.push_back(a.dna[i]);
        }
        for (int i = pivot; i < dnaSize; i++)
        {
            dna.push_back(b.dna[i]);
        }
        fitness = 0;
    }

    DNA::DNA(int n, float x, float y, float width, int height, float Sx, float Sy)
    {
        
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;
        sx_ = Sx;
        sxx_ = Sy;
        dnaSize = n;
        initDNA();
    }
    DNA::~DNA()
    {
    }
    void DNA::initDNA()
    {
        for (int i = 0; i < dnaSize; i++)
        {
            dna.push_back(Brush());
            set_rotate(dna.back());
            set_brightness(dna.back());
            set_brushidx(dna.back());
            set_scale(dna.back());
            set_translate(dna.back());
        }
    }

    bool DNA::operator<(DNA &a)
    {
        return fitness < a.fitness;
    }
    void DNA::set_rotate(Brush &brush)
    {
        brush.set_rotate(getRandFloat(0.0f, 360.0f));
    }
    void DNA::set_brightness(Brush &brush)
    {
        brush.set_brightness(getRandFloat(0.0f, 1.0f));
    }
    void DNA::set_brushidx(Brush &brush)
    {
        brush.set_brushidx(rand()%Brush::get_brushes_size());
    }
    void DNA::set_scale(Brush &brush)
    {
        float sxyz[3] = {
            getRandFloat(sx_, sxx_), 
            getRandFloat(sx_, sxx_),
            1.0f
        };
        brush.set_scale(sxyz);
    }
    void DNA::set_translate(Brush &brush)
    {
        float txyz[3] = {
            getRandFloat(-width_ / 2.0f + x_, width_ / 2.0f + x_),
            getRandFloat(-height_ / 2.0f + y_, height_ / 2.0f + y_),
            -getRandFloat(2.0f, 50.0f)
        };
        brush.set_translate(txyz);
    }
    void DNA::mutate()
    {
        for (int i = 0; i < dnaSize; i++)
        {
            if (rand() % 10 == 0)
            {
                if (rand() % 2 == 0)
                {
                    set_rotate(dna[i]);
                }
                if (rand() % 2 == 0)
                {
                    set_brightness(dna[i]);
                }
                if (rand() % 2 == 0)
                {
                    set_brushidx(dna[i]);
                }
                if (rand() % 2 == 0)
                {
                    set_scale(dna[i]);
                }
                if (rand() % 2 == 0)
                {
                    set_translate(dna[i]);
                }
            }
        }
    }
    DNA *DNA::crossover(DNA &a)
    {
        return new DNA(a, a);
    }
    void DNA::draw(int i)
    {
        dna[i].draw();
    }
    void DNA::drawAll()
    {
        for (int i = 0; i < dnaSize; i++)
        {
            this->draw(i);
        }
    }

    int DNA::size()
    {
        return dnaSize;
    }

    double &DNA::fitnessRef()
    {
        return fitness;
    }

}
namespace nsg {
    GLubyte **DNA::getPicture()
    {
        WindowControl::rendering_lock();
        glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
        glDrawBuffer(GL_BACK);
        WindowControl::g_windows_[1]->window_clear_white();
        //drawing back()
        drawAll();
        GLubyte **ret = WindowControl::g_windows_[1]->get_window_pixel();
        WindowControl::rendering_unlock();
        return ret;
    }
    float getRandFloat(float lo, float hi)
{
    return lo + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (hi - lo)));
}
}