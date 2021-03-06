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
        int relative_height_;
        int relative_width_;
        GLenum pixel_format_;
        GLubyte *image_data_;
        int image_size_;
        int padding_;

    public:
        Picture(int width, int height, int byte_per_pixel, GLenum pixel_format);
        Picture(const char *filepath, int byte_per_pixel);
        virtual ~Picture();
        void init_picture();
        void init_pbo();
        void prepare_sub_picture();
        void start_sub_picture();
        void start_sub_picture(int posx, int posy, int width, int height);
        void read_back_buffer(int posx, int posy, int width, int height);
        void read_back_buffer();
        void unbind_write_pbo();
        void set_width(int width);
        void set_height(int height);
        int get_relative_width();
        int get_relative_height();
    };
}

#endif
