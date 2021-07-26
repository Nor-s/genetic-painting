#ifndef MY_PAINTING_H
#define MY_PAINTING_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>

#include "learnopengl/stb_image.h"
#include "learnopengl/shader_s.h"

namespace nsg {
    class myPainting {
    private:
        float vertices[20] = {
            // positions              // texture coords
             0.25f,  0.25f, 0.0f,   1.0f, 1.0f,   // top right
             0.25f, -0.25f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.25f, -0.25f, 0.0f,   0.0f, 0.0f,   // bottom left
            -0.25f,  0.25f, 0.0f,   0.0f, 1.0f    // top left 
        };
        unsigned int indices[6] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };
        const char* tex[4] = {
            "././resources/3.jpg",
            "././resources/2.jpg",
            "././resources/3.jpg",
            "././resources/4.jpg",
        };
        const char* vertexShader = "././shaders/vs_brush.vs";
        const char* fragmentShader = "././shaders/fs_brush.fs";
        unsigned int texture;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        Shader* shader;
        float currentColor[4];
        glm::mat4 transform;

    public:
        myPainting();
        ~myPainting();
        unsigned int initTexture(const char* fileName, GLint internalFormat, GLenum originalFormat, GLenum originalType);
        void initObject(unsigned int* VBO, unsigned int* VAO, unsigned int* EBO);
        void initTextureUnit();
        void initTransform();
        void setTransformToUniform();
        void setTransformToRand();
        void setColor();
        void translate(float tx, float ty, float tz);
        void rotate(float degree);
        void scale(float sx, float sy, float sz);
        void draw();

        float getRandFloat(float lo, float hi);
    };
}
#endif

