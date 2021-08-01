#include "myheader/picture.h"

namespace nsg
{
    Picture::Picture(int width, int height)
    {
    }
    Picture::Picture(const char *filepath) : SquareObject(filepath)
    {
        init_shader(vs_grayscale_shader_, fs_grayscale_shader_);
    }
    Picture::~Picture() {}
    void Picture::draw()
    {
        if (is_pixel_picture_)
        {
        }
        else
        {
            SquareObject::draw();
        }
    }
}