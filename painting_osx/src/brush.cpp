#include "myheader/brush.h"

namespace nsg {
    Brush::Brush(int idx) {
        init_texture(tex_[idx], GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
        set_vertices(tex_height_, tex_height_);
        init_buffer_objects();
        init_shader();
    }
}

namespace nsg
{
    std::vector<Brush *> Palette::brushes_;
    int Palette::brushes_size_ = 0;
    Palette::Palette() {}
    void Palette::init_brushes()
    {
        if (Palette::brushes_size_ == 0)
        {
            for (int i = 0; i < 4; i++)
            {
                Palette::brushes_.push_back(new Brush(i));
                Palette::brushes_size_++;
            }
        }
    }
    int Palette::get_brushes_size()
    {
        return brushes_size_;
    }
    void Palette::set_translate(float translate[3])
    {
        for (int i = 0; i < 3; i++)
        {
            translate_[i] = translate[i];
        }
    }
    void Palette::set_scale(float scale[3])
    {
        for (int i = 0; i < 3; i++)
        {
            scale_[i] = scale[i];
        }
    }
    void Palette::set_rotate(float rotate)
    {
        rotate_ = rotate;
    }
    void Palette::set_brightness(float brightness)
    {
        brightness_ = brightness;
    }
    void Palette::set_brushidx(int idx)
    {
        idx_ = idx;
    }
    void Palette::draw()
    {
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