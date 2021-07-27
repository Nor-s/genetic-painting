#include "myheader/myPainting.h"

namespace nsg {
    myPainting::myPainting() {
        stbi_set_flip_vertically_on_load(true);
        initObject(&VAO, &VBO, &EBO);
        texture = initTexture(tex[rand()%10], GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
        shader = new Shader(vertexShader, fragmentShader);
        initTextureUnit();
        initTransform();
        setTransformToRand();
        setTransformToUniform();
        
        setBrightToRand();
        setBrightToUniform();
    }
    //maybe not safe
    myPainting::~myPainting() {
        glDeleteTextures(1, &texture);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
 //       delete shader;
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
    void myPainting::setTransformToUniform() {
        shader->use();
        shader->setMat4("transform", transform);
    }
    void myPainting::setTransformToRand() {
        float randT[2] = {getRandFloat(-1.0f, 1.0f), getRandFloat(-1.0f, 1.0f)};
        float randS = getRandFloat(0.15f, 0.25f);
        float randDegree = getRandFloat(0.0f, 360.0f);

        translate(randT[0], randT[1], 0.0f);
        scale(randS, randS, 0.0f);
        rotate(randDegree);
    }
    void myPainting::setBrightToRand() {
        bright = getRandFloat(0.0, 1.0);
    }
    void myPainting::setBrightToUniform() {
        shader->use();
        shader->setFloat("bright", bright);
    }
    void myPainting::translate(float tx, float ty, float tz) {
        transform = glm::translate(transform, glm::vec3(tx, ty, tz));
    }
    void myPainting::rotate(float degree) {
        transform = glm::rotate(transform, glm::radians(degree) ,glm::vec3(0.0f, 0.0f, 1.0f));
    }
    void myPainting::scale(float sx, float sy, float sz) {
        transform = glm::scale(transform, glm::vec3(sx, sy, sz));
    }
    void myPainting::setColor() {
        return;
    }
    float myPainting::getRandFloat(float lo, float hi) {
        return lo + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(hi-lo)));
    }
   // void myPainting::
   // void myPainting::
}