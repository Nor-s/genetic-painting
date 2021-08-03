#include "myheader/square_object.h"

glm::mat4 Shader::projection_matrix;

namespace nsg
{
    bool SquareObject::g_is_grayscale_ = false;
    SquareObject::SquareObject() {}
    SquareObject::~SquareObject()
    {
#ifdef DEBUG_MOD
        std::cout << "~~~~painting\n";
#endif
        glDeleteTextures(1, &texture_id_);
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ebo_);
        delete shader_;
    }
    void SquareObject::init_shader()
    {
        init_shader(vs_base_, fs_base_);
    }
    void SquareObject::init_shader(const char *vertex, const char *frag)
    {
        shader_ = new Shader(vertex, frag);
        init_texture_unit();
        init_model();
        set_model_to_uniform();
        set_color_to_uniform(1.0f, 1.0f, 1.0f, 1.0f);
    }
    void SquareObject::init_texture(GLvoid *image_data, int width, int height, GLenum pixel_format)
    {
        // init texture objects
        glGenTextures(1, &texture_id_);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, pixel_format, GL_UNSIGNED_BYTE, image_data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void SquareObject::init_texture(const char *fileName, GLint inter_format, GLenum origin_format, GLenum origin_type)
    {
        stbi_set_flip_vertically_on_load(true);
        glGenTextures(1, &texture_id_);
        glBindTexture(GL_TEXTURE_2D, texture_id_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int nr_channels;
        unsigned char *data = stbi_load(fileName, &tex_width_, &tex_height_, &nr_channels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, inter_format, tex_width_, tex_height_, 0, origin_format, origin_type, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        
#ifdef __APPLE__
        while (tex_width_ > 100 || tex_height_ > 100) {
            tex_width_ /= 2;
            tex_height_ /= 2;
        }
#endif
        while (tex_width_ > 800 || tex_height_ > 800)
        {
            tex_width_ *= 0.5f;
            tex_height_ *= 0.5f;
        }
        
        
        stbi_image_free(data);
    }
    void SquareObject::init_buffer_objects()
    {
        //Gen
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);

        //Bind
        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

        //Buffer -> Data
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);

        //vertexAttrib : pos
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        //vertexAttrib : texture coord
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        //unBind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind order : VAO -> EBO
    }
    void SquareObject::init_texture_unit()
    {
        shader_->use();
        shader_->setInt("Texture0", 0);
    }
    void SquareObject::draw()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_);

        shader_->use();
        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    void SquareObject::init_model()
    {
        model_transform_ = glm::mat4(1.0f);
    }
    void SquareObject::set_vertices()
    {
        vertices_[0] = vertices_[5] = (float)tex_width_;
        vertices_[10] = vertices_[15] = 0.0f;
        vertices_[1] = vertices_[16] = (float)tex_height_;
        vertices_[6] = vertices_[11] = 0.0f;
    }
    void SquareObject::set_vertices(int width, int height)
    {
        vertices_[0] = vertices_[5] = (float)width;
        vertices_[10] = vertices_[15] = 0.0f;
        vertices_[1] = vertices_[16] = (float)height;
        vertices_[6] = vertices_[11] = 0.0f;
    }
    void SquareObject::set_model_to_uniform()
    {
        shader_->use();
        shader_->setMat4("Model", model_transform_);
    }
    void SquareObject::set_color_to_uniform(float r, float g, float b, float a)
    {
        shader_->use();
        shader_->setVec4("Color", r, g, b, a);
    }
    void SquareObject::set_color_to_uniform(float color)
    {
        shader_->use();
        shader_->setVec4("Color", color, color, color, color);
    }
    void SquareObject::set_color_to_uniform(float color[4])
    {
        shader_->use();
        shader_->setVec4("Color", color[0], color[1], color[2], color[3]);
    }
    int SquareObject::get_tex_width()
    {
        return tex_width_;
    }
    int SquareObject::get_tex_height()
    {
        return tex_height_;
    }
    void SquareObject::translate(float t[3])
    {
        model_transform_ = glm::translate(model_transform_, glm::vec3(t[0], t[1], t[2]));
    }
    void SquareObject::translate(float tx, float ty, float tz)
    {
        model_transform_ = glm::translate(model_transform_, glm::vec3(tx, ty, tz));
    }
    void SquareObject::rotate(float degree)
    {
        model_transform_ = glm::rotate(model_transform_, glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    void SquareObject::scale(float s)
    {
        model_transform_ = glm::scale(model_transform_, glm::vec3(s, s, s));
    }
    void SquareObject::scale(float s[3])
    {
        model_transform_ = glm::scale(model_transform_, glm::vec3(s[0], s[1], s[2]));
    }
    void SquareObject::scale(float sx, float sy, float sz)
    {
        model_transform_ = glm::scale(model_transform_, glm::vec3(sx, sy, sz));
    }
}
