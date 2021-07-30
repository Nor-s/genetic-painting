#include "myheader/myPainting.h"

namespace nsg {
    myPainting::myPainting(int idx) {
        texture = initTexture(tex[idx], GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
        setVertices(false);
        initObject();
        shader = new Shader(vertexShader, fragmentShader);
        initTextureUnit();
        initTransform();
        translate(0.0f, 0.0f, 0.0f);
        setTransformToUniform();
        setBrightToUniform(1.0);
    }
    myPainting::myPainting(const char* filePath) {
        stbi_set_flip_vertically_on_load(true);
        texture = initTexture(filePath, GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE);
        setVertices(false);
        initObject();
        shader = new Shader(grayVertexShader, grayFragmentShader);
        initTextureUnit();
        initTransform();
        translate(0.0f, 0.0f, 0.0f);
        setTransformToUniform();
        setBrightToUniform(1.0f);
    }
    myPainting::~myPainting() {
    #ifdef DEBUG_MOD
        std::cout<<"~~~~painting\n";
    #endif
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int nrChannels;
        unsigned char *data = stbi_load(fileName, &texWidth, &texHeight, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texWidth, texHeight, 0, originalFormat, originalType, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
    #ifdef __APPLE__
        texWidth*=0.7, texHeight *=0.7;
    #endif
        stbi_image_free(data);
        return ID;
    }
    void myPainting::initObject() {
        //Gen
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        //Bind
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        //Buffer -> Data 
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
    void myPainting::initTextureUnit() {
        shader->use();
        shader->setInt("texture0", 0);
    }
    void myPainting::draw() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        shader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    void myPainting::initTransform() {
        transform = glm::mat4(1.0f);
    }
    void myPainting::setVertices (bool isBrush) {
        if(!isBrush) {
            vertices[0] = vertices[5] = (float)texWidth/2.0f;
            vertices[10] = vertices[15] = -((float)texWidth)/2.0f;
            vertices[1] = vertices[16] = (float)texHeight/2.0f;
            vertices[6] = vertices[11] = -((float)texHeight/2.0f);
        }
        else {
            vertices[0] = vertices[5] = (float)texHeight/2.0f;
            vertices[10] = vertices[15] = -((float)texHeight)/2.0f;
            vertices[1] = vertices[16] = (float)texHeight/2.0f;
            vertices[6] = vertices[11] = -((float)texHeight/2.0f);
        }
    }
    void myPainting::setProjectionToUniform(const glm::mat4& projection) {
        shader->use();
        shader->setMat4("projection", projection);
    }
    void myPainting::setTransformToUniform() {
        shader->use();
        shader->setMat4("model", transform);
    }
    void myPainting::setBrightToUniform(float bright) {
        shader->use();
        shader->setFloat("bright", bright);
    }
    void myPainting::translate(float t[3]) {
        transform = glm::translate(transform, glm::vec3(t[0], t[1], t[2]));
    }
    void myPainting::translate(float tx, float ty, float tz) {
        transform = glm::translate(transform, glm::vec3(tx, ty, tz));
    }
    void myPainting::rotate(float degree) {
        transform = glm::rotate(transform, glm::radians(degree) ,glm::vec3(0.0f, 0.0f, 1.0f));
    }
    void myPainting::scale(float s) {
        transform = glm::scale(transform, glm::vec3(s, s, s));
    }
    void myPainting::scale(float sx, float sy, float sz) {
        transform = glm::scale(transform, glm::vec3(sx, sy, sz));
    }
    void myPainting::setColor() {
        return;
    }

}