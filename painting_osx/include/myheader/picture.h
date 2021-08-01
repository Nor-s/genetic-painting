#ifndef MY_PICTURE_H
#define MY_PICTURE_H

#include "myheader/square_object.h"

namespace nsg
{
    class Picture : virtual public SquareObject
    {
    private:
        const char *vs_grayscale_shader_ = "../shaders/vs_gray.vs";
        const char *fs_grayscale_shader_ = "../shaders/fs_gray.fs";
        unsigned int pbo_ids_[2];
        bool is_pixel_picture_;

    public:
        Picture(int width, int height);
        Picture(const char *filepath);
        virtual ~Picture();
        virtual void draw();
        void init_picture();
        void update_picture();
        void init_pbo();
    };
}

#endif
