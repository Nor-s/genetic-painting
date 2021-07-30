#include "myheader/square_object2d.h"

namespace genetic_painting {
    SquareObject2D::SquareObject2D() {}
    SquareObject2D::~SquareObject2D() {
    #ifdef DEBUG_MODE
        std::cout<<"Destruction : SquareObject2D\n";
    #endif
    std::cout<<"sdfdsfdsf";
        glDeleteTextures(1, &texture_);
        glDeleteVertexArrays(1, &vertex_array_object_);
        glDeleteBuffers(1, &vertex_buffer_object_);
        glDeleteBuffers(1, &element_buffer_object_);
        delete shader_;
    }
}

namespace genetic_painting {
    unsigned int SquareObject2D::init_texture(const char* filepath, GLint internal_format, GLenum original_format, GLenum original_type) {
     stbi_set_flip_vertically_on_load(true);

        unsigned int ID;
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int nrChannels;
        unsigned char *data = stbi_load(filepath, &texture_width_, &texture_height_, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, texture_width_, texture_height_, 0, original_format, original_type, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
    #ifdef __APPLE__
        texture_width_*=0.7, texture_height_ *=0.7;
    #endif
        stbi_image_free(data);
        return ID;
    }
    void SquareObject2D::init_square_object2d() {
        rgba_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        brightness_ = 1.0f;
        set_rgba();
        set_brightness();
        init_transform_matrixs();
    }
    void SquareObject2D::init_buffer_objects() {
        //Gen
        glGenVertexArrays(1, &vertex_array_object_);
        glGenBuffers(1, &vertex_buffer_object_);
        glGenBuffers(1, &element_buffer_object_);

        //Bind
        glBindVertexArray(vertex_array_object_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);

        //Buffer -> Data 
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);

        //vertexAttrib : pos
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //vertexAttrib : texture coord
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        //unBind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind order : VAO -> EBO
    }
    void SquareObject2D::init_transform_matrixs() {
        view_transform_ = glm::mat4(1.0f);
        projection_transform_ = glm::mat4(1.0f);
        model_transform_ = glm::mat4(1.0f);
        set_view_transform();
        set_projection_transform();
        set_model_transform();
    }
}

namespace genetic_painting {
    void SquareObject2D::set_vertices(GLuint left_x, GLuint bottom_y, GLsizei right_x, GLsizei top_y) {
        float x_vertice = static_cast<float>(right_x - left_x)/2.0f;
        float y_vertice = static_cast<float>(top_y - bottom_y)/2.0f;
        vertices_[0] = vertices_[5] = x_vertice;
        vertices_[10] = vertices_[15] = -x_vertice;
        vertices_[1] = vertices_[16] = y_vertice;
        vertices_[6] = vertices_[11] = -y_vertice;
    }
    void SquareObject2D::set_texture_unit() {
        shader_->use();
        shader_->setInt("texture0", 0);
    }    
    void SquareObject2D::set_rgba(glm::vec4 rgba) {
        rgba_ = rgba;
        set_rgba();
    }
    void SquareObject2D::set_brightness(const GLfloat brightness) {
        brightness_ = brightness;
        set_brightness();
    }
    void SquareObject2D::set_model_transform(const glm::mat4& model_transform) {
        model_transform_ = model_transform;
        set_model_transform();
    }
    void SquareObject2D::set_projection_transform(const glm::mat4& projection_transform) {
        projection_transform_ = projection_transform;
        set_projection_transform();
    }
    void SquareObject2D::set_view_transform(const glm::mat4& view_transform) {
        view_transform_ = view_transform;
        set_view_transform();
    }
}
/*
    set rgba, brightness, projection, view, model mat using shader
*/
namespace genetic_painting {
    void SquareObject2D::set_rgba() {
        shader_->use();
        shader_->setVec4("Color", rgba_);
    }
    void SquareObject2D::set_brightness() {
        shader_->use();
        shader_->setFloat("Bright", brightness_);
    }
    void SquareObject2D::set_model_transform() {
        shader_->use();
        shader_->setMat4("Model", model_transform_);
    }
    void SquareObject2D::set_projection_transform() {
        shader_->use();
        shader_->setMat4("Projection", projection_transform_);
    }
    void SquareObject2D::set_view_transform() {
        shader_->use();
        shader_->setMat4("View", view_transform_);
    }
}

namespace genetic_painting {
    int SquareObject2D::get_texture_width() {
        return texture_width_;
    }
    int SquareObject2D::get_texture_height() {
        return texture_height_;
    }
}

namespace genetic_painting {
    void SquareObject2D::translate(GLfloat x, GLfloat y, GLfloat z) {
        model_transform_= glm::translate(model_transform_ , glm::vec3(x, y, z));
    }
    void SquareObject2D::rotate(GLfloat degree) {
        model_transform_ = glm::rotate(model_transform_ , glm::radians(degree) ,glm::vec3(0.0f, 0.0f, 1.0f));
    }
    void SquareObject2D::scale(GLfloat sx, GLfloat sy, GLfloat sz) {
        model_transform_ = glm::scale(model_transform_, glm::vec3(sx, sy, sz));
    }
    void SquareObject2D::draw() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_);
        
        shader_->use();
        glBindVertexArray(vertex_array_object_);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}