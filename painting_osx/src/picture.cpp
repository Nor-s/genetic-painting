#include "myheader/picture.h"
//#define DEBUG_MODE
namespace nsg
{
    Picture::Picture(int width, int height, int byte_per_pixel, GLenum pixel_format)
    {
        tex_width_ = width;
        tex_height_ = height;
        pixel_format_ = pixel_format;
        byte_per_pixel_ = byte_per_pixel;
        image_size_ = (width * byte_per_pixel_ + width % 4) * height;
        image_data_ = new GLubyte[image_size_];
        memset(image_data_, 0, image_size_);
        SquareObject::init_texture((GLvoid *)image_data_, width, height, pixel_format);
        SquareObject::set_vertices();
        SquareObject::init_buffer_objects();
        init_pbo();
        SquareObject::init_shader();
        SquareObject::translate(0.0f, 0.0f, -99.0f);
        SquareObject::set_model_to_uniform();
    }

    Picture::Picture(const char *filepath) : SquareObject(filepath)
    {
        image_data_ = nullptr;
        SquareObject::init_shader(vs_grayscale_shader_, fs_grayscale_shader_);
    }
    Picture::~Picture()
    {
        if (image_data_ != nullptr)
        {
            delete[] image_data_;
        }
        glDeleteBuffers(1, &write_pbo_id_);
    }

    void Picture::init_pbo()
    {
        // create 2 pixel buffer objects, you need to delete them when program exits.
        // glBufferData() with NULL pointer reserves only memory space.
        glGenBuffers(1, &write_pbo_id_);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, write_pbo_id_);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, image_size_, 0, GL_STREAM_DRAW);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }
    void Picture::bind_write_pbo_pointer()
    {
        // bind PBO
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, write_pbo_id_);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, image_size_, 0, GL_STREAM_DRAW);
        image_data_ = (GLubyte *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    }
    void Picture::read_back_buffer()
    {
        glReadPixels(
            0, 0,
            tex_width_, tex_height_,
            ((byte_per_pixel_ == 3) ? GL_BGR : GL_BGRA),
            GL_UNSIGNED_BYTE,
            image_data_);
    }
    void Picture::unbind_write_pbo()
    {
        if (image_data_)
        {
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // release pointer to mapping buffer
        }
#ifdef DEBUG_MODE
        else
        {
            std::cout << "error : picture.cpp -> unbind_write_pbo\n";
        }
#endif
    }
    void Picture::sub_picture()
    {
        // bind the texture and PBO
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, write_pbo_id_);

        // copy pixels from PBO to texture object
        // Use offset instead of ponter.
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex_width_, tex_height_, pixel_format_, GL_UNSIGNED_BYTE, 0);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }
}