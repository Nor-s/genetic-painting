#include "myheader/picture.h"
//#define DEBUG_MODE
namespace nsg
{
    /*
        bitmap picture
        init : black 
         - - - sub_picture order - - -

            bind_write_pbo_pointer();
                |
            (rendering)
                |
            read_back_buffer();
                |
            unbind_write_pbo();
                |
            sub_picture();
         - - - - - - - - - - - - - - - 
    */
    Picture::Picture(int width, int height, int byte_per_pixel, GLenum pixel_format)
    {
        set_width(width);
        set_height(height);

        pixel_format_ = pixel_format;
        byte_per_pixel_ = byte_per_pixel;
        image_size_ = (relative_width_ * byte_per_pixel_ + relative_width_ % 4) * relative_height_;

        image_data_ = new GLubyte[image_size_];
        memset(image_data_, 0, image_size_);
        SquareObject::init_texture((GLvoid *)image_data_, relative_width_, relative_height_, pixel_format);

        SquareObject::set_vertices();
        SquareObject::init_buffer_objects();
        init_pbo();
        SquareObject::init_shader();
        SquareObject::translate(0.0f, 0.0f, -50.0f);
        SquareObject::set_model_to_uniform();
    }
    
    Picture::Picture(const char *filepath, int byte_per_pixel)
    {
        SquareObject::init_texture(filepath, GL_RGBA, (byte_per_pixel == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE);

        byte_per_pixel_ = byte_per_pixel;
        image_data_ = nullptr;
        SquareObject::set_vertices();
        init_buffer_objects();
        SquareObject::init_shader();
        SquareObject::translate(0.0f, 0.0f, -50.0f);
        SquareObject::set_model_to_uniform();
        if (g_is_grayscale_)
        {
            set_color_to_uniform(-1.0f);
        }
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
    void Picture::prepare_sub_picture()
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
            get_relative_width(), get_relative_height(),
            ((byte_per_pixel_ == 3) ? GL_BGR : GL_BGRA),
            GL_UNSIGNED_BYTE,
            image_data_);
    }
    void Picture::unbind_write_pbo()
    {
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // release pointer to mapping buffer
    }
    void Picture::start_sub_picture()
    {
        read_back_buffer();
        unbind_write_pbo();
        // bind the texture and PBO
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, write_pbo_id_);

        // copy pixels from PBO to texture object
        // Use offset instead of ponter.
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, relative_width_, relative_height_, pixel_format_, GL_UNSIGNED_BYTE, 0);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }
    void Picture::set_width(int width)
    {
        relative_width_ = tex_width_ = width;
#ifdef __APPLE__
 //      relative_width_ *= 2;
#endif
    }
    void Picture::set_height(int height)
    {
        relative_height_ = tex_height_ = height;
#ifdef __APPLE__
   //     relative_height_ *= 2;
#endif
    }
    int Picture::get_relative_width()
    {
        return relative_width_;
    }
    int Picture::get_relative_height()
    {
        return relative_height_;
    }
}