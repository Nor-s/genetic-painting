#ifndef MY_SQUARE_OBJECT_H
#define MY_SQUARE_OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>

#include "learnopengl/stb_image.h"
#include "learnopengl/shader_s.h"

namespace nsg {
    class SquareObject {
    private:
    //5 2
        float vertices_[20] = {
            // positions              // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
        };
        unsigned int indices_[6] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };
        const char* tex_[4] = {
            "../resources/1.png",
            "../resources/2.png",
            "../resources/3.png",
            "../resources/4.png",
        };
        const char* vs_shader_ = "../shaders/vs_brush.vs";
        const char* fs_shader_ = "../shaders/fs_brush.fs";
        const char* vs_grayscale_shader_ = "../shaders/vs_gray.vs";
        const char* fs_grayscale_shader_ = "../shaders/fs_gray.fs";
        unsigned int texture_id_;
        unsigned int vao_;
        unsigned int vbo_;
        unsigned int ebo_;
        int tex_width_;
        int tex_height_;
        Shader* shader_;
        glm::mat4 model_transform_;
    public:
        SquareObject(int idx);
        SquareObject(const char* filepath);
        ~SquareObject();
        unsigned int init_texture(const char* filepath, GLint inter_format, GLenum origin_format, GLenum origin_type);
        void init_square_object(const char* tex, GLint inter_format, GLenum origin_format, GLenum origin_type);
        void init_buffer_objects();
        void init_texture_unit();
        void init_model();
        void set_vertices (int width, int height);
        void set_projection_to_uniform(const glm::mat4& projection);
        void set_model_to_uniform();
        void set_bright_to_uniform(float bright);
        int  get_tex_width();
        int  get_tex_height();
        void translate(float t[3]);
        void translate(float tx, float ty, float tz);
        void rotate(float degree);
        void scale(float s);
        void scale(float sx, float sy, float sz);
        void draw();
    };
}
#endif
