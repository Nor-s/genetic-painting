#ifndef MY_PICTURE_H
#define MY_PICTURE_H

#include <cstring>
#include "myheader/square_object.h"

namespace nsg
{
    class Picture : virtual public SquareObject
    {
    private:
        const char *vs_grayscale_shader_ = "../shaders/vs_gray.vs";
        const char *fs_grayscale_shader_ = "../shaders/fs_gray.fs";
        unsigned int byte_per_pixel_;
        unsigned int write_pbo_id_;
        GLenum pixel_format_;
        GLubyte* image_data_;
        int image_size_;
    public:
        Picture(int width, int height, int channel_count, GLenum pixel_format);
        Picture(const char *filepath);
        virtual ~Picture();
        void init_picture();
        void sub_picture();
        void init_pbo();
        void bind_write_pbo_pointer();
        void read_back_buffer();
        void unbind_write_pbo();
    };
}

#endif
