#include <stdlib.h>
#include <time.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "myheader/myWindow.h"
#include "myheader/myPainting.h"

void processInput1(nsg::myWindow* window, std::vector<nsg::myPainting*>& canvas);

std::vector<nsg::myPainting*> canvas;

int main() {
    srand (static_cast <unsigned> (time(0)));  
    glfwInit();
    nsg::myWindow window(800, 800);

    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    while(!glfwWindowShouldClose(window.getWindow())) {
        processInput1(&window, canvas);

        window.windowClear(GL_COLOR_BUFFER_BIT, 0.0f, 0.0f, 0.0f, 1.0f);

        for(int i = 0; i < static_cast<int>(canvas.size()); i++) {
            canvas[i]->setProjectionToUniform(window.projection);
            canvas[i]->draw();
        }

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput1(nsg::myWindow *window, std::vector<nsg::myPainting*>& canvas) {
    static int count = 1;
    if (glfwGetKey(window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window->getWindow(), true);
    }
    if (glfwGetKey(window->getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        canvas.push_back(new nsg::myPainting());
    }
    if (glfwGetKey(window->getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        if(count == 0) {
            count = 1;
        }
    }
    if (glfwGetKey(window->getWindow(), GLFW_KEY_1) == GLFW_PRESS) {
        if(count == 0) {
            return;
        }
        count--;
        window->windowCapture("sdf");
    }
}