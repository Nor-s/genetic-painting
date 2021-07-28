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
    //5 2
        float vertices[20] = {
            // positions              // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
        };
        unsigned int indices[6] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };
        const char* tex[4] = {
            "../resources/1.png",
            "../resources/2.png",
            "../resources/3.png",
            "../resources/4.png",
        };
        const char* vertexShader = "../shaders/vs_brush.vs";
        const char* fragmentShader = "../shaders/fs_brush.fs";
        const char* grayVertexShader = "../shaders/vs_gray.vs";
        const char* grayFragmentShader = "../shaders/fs_gray.fs";
        unsigned int texture;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        Shader* shader;
        float currentColor[4];
        glm::mat4 transform;
    public:
        int texWidth;
        int texHeight;
        myPainting(float x, float y, float width, float height, float Sx, float Sy);
        myPainting(const char* filePath);
        ~myPainting();
        unsigned int initTexture(const char* fileName, GLint internalFormat, GLenum originalFormat, GLenum originalType);
        void initObject();
        void initTextureUnit();
        void initTransform();
        void setVertices (bool isBrush);
        void setProjectionToUniform(const glm::mat4& projection);
        void setTransformToUniform();
        // viewport coordinate and size scale
        void setTransformToRand(float x, float y, float width, float height, float Sx, float Sy);
        void setBrightToUniform(float bright);
        void setColor();
        void translate(float tx, float ty, float tz);
        void rotate(float degree);
        void scale(float sx, float sy, float sz);
        void draw();

        static float getRandFloat(float lo, float hi);
    };
}
#endif

