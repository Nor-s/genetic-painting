#include "myheader/myPainting.h"

namespace nsg {
    myPainting::myPainting() {
        stbi_set_flip_vertically_on_load(true);
        initObject(&VAO, &VBO, &EBO);
        texture = initTexture(tex[2], GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
        shader = new Shader(vertexShader, fragmentShader);
        initTextureUnit();
    }
    myPainting::~myPainting() {
        glDeleteTextures(1, &texture);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        delete shader;
    }
    unsigned int myPainting::initTexture(const char* fileName, GLint internalFormat, GLenum originalFormat, GLenum originalType) {
        unsigned int ID;
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, originalFormat, originalType, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);
        return ID;
    }
    void myPainting::initObject(unsigned int* VBO, unsigned int* VAO, unsigned int* EBO) {
        //Gen
        glGenVertexArrays(1, VAO);
        glGenBuffers(1, VBO);
        glGenBuffers(1, EBO);

        //Bind
        glBindVertexArray(*VAO);
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);

        //Buffer -> Data 
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //vertexAttrib : pos
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //vertexAttrib : color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        //vertexAttrib : texture coord
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);

        //unBind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind order : VAO -> EBO
    }
    void myPainting::initTextureUnit() {
        shader->use();
        shader->setInt("texture", 0);
    }
    void myPainting::draw() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        shader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}