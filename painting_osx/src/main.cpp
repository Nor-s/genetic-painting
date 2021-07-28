#include <stdlib.h>
#include <time.h>
#include <vector>

#define DEPTH_TEST

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "myheader/myWindow.h"
#include "myheader/myPainting.h"

void drawDisplay0();
void drawDisplay1();
void processInput(std::vector<nsg::myPainting*>& canvas);
void drop_callback(GLFWwindow* window, int count, const char** paths);
void windowResizeCallback(GLFWwindow* windowPointer, int width, int height);
void pos_callback(GLFWwindow* window, int x, int y);

std::vector<nsg::myPainting*> canvas;
std::vector<nsg::myWindow*> windows;
nsg::myPainting* picture;

int main() {
    srand (static_cast <unsigned> (time(0)));  

    glfwInit();
    windows.push_back(new nsg::myWindow(800, 800, "painting"));
    
#ifdef DEPPTH_TEST
    glEnable(GL_DEPTH_TEST);
#endif
    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    //callback
    glfwSetDropCallback(windows[0]->getWindow(), drop_callback);
    glfwSetFramebufferSizeCallback(windows[0]->getWindow(), windowResizeCallback);
	glfwSetWindowPosCallback(windows[0]->getWindow(), pos_callback);

    while(!glfwWindowShouldClose(windows[0]->getWindow())) {
        processInput(canvas);
        drawDisplay1();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void drawDisplay1(){
    if(windows.size() == 2) {
        glfwMakeContextCurrent(windows[1]->getWindow());
        windows[1]->windowClear(GL_COLOR_BUFFER_BIT, 0.0f, 0.0f, 0.0f, 1.0f);

        for(int i = 0; i < static_cast<int>(canvas.size()); i++) {
            canvas[i]->setProjectionToUniform(windows[1]->projection);
            canvas[i]->draw();
        }
        if(canvas.size() < 1000) {
            canvas.push_back(new nsg::myPainting(nsg::myWindow::SCR_WIDTH, nsg::myWindow::SCR_HEIGHT));
        }

        glfwSwapBuffers(windows[1]->getWindow());
    }
    else {
        windows[0]->windowClear(GL_COLOR_BUFFER_BIT, 0.0f, 0.2f, 0.2f, 1.0f);
        glfwSwapBuffers(windows[0]->getWindow());
    }
}

void drawDisplay0() {
    if(windows.size() == 2) {
        glfwMakeContextCurrent(windows[0]->getWindow());

        windows[0]->windowClear(GL_COLOR_BUFFER_BIT, 0.0f, 0.2f, 0.2f, 1.0f);
        picture->setProjectionToUniform(nsg::myWindow::projection);
        picture->draw();
        glfwSwapBuffers(windows[0]->getWindow());
    }
}

void processInput(std::vector<nsg::myPainting*>& canvas) {
    static int count = 1;
    if (glfwGetKey(windows[0]->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(windows[0]->getWindow(), true);
    }
    if (glfwGetKey(windows[0]->getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        if(count == 0) {
            count = 1;
        }
    }
    if (windows.size() == 2 && glfwGetKey(windows[1]->getWindow(), GLFW_KEY_1) == GLFW_PRESS) {
        if(count == 0) {
            return;
        }
        count--;
        windows[1]->windowCapture("sdf");
    }
}

void drop_callback(GLFWwindow* window, int count, const char** paths)
{
    if(windows.size() == 2) {
        return;
    }
    int i;
    for (i = 0;  i < 1;  i++) {
        picture = new nsg::myPainting(paths[i]);

        glfwSetWindowSize(windows[0]->getWindow(), picture->texWidth, picture->texHeight);

        windows.push_back(new nsg::myWindow(picture->texWidth, picture->texHeight, "picture"));
        glfwSetFramebufferSizeCallback(windows[1]->getWindow(), windowResizeCallback);
	    glfwSetWindowPosCallback(windows[1]->getWindow(), pos_callback);
        drawDisplay0();
    }
}

void windowResizeCallback(GLFWwindow* windowPointer, int width, int height) {
    glViewport(0, 0, width, height);
    float widthFactor = static_cast<float>(width) / static_cast<float>(nsg::myWindow::SCR_WIDTH);
    float heightFactor = static_cast<float>(height) / static_cast<float>(nsg::myWindow::SCR_HEIGHT);

    nsg::myWindow::projection = glm::ortho(
         -(float)nsg::myWindow::SCR_WIDTH/2.0f * widthFactor, (float)nsg::myWindow::SCR_WIDTH/2.0f * widthFactor, 
         -(float)nsg::myWindow::SCR_HEIGHT/2.0f * heightFactor, (float)nsg::myWindow::SCR_HEIGHT/2.0f * heightFactor, 
        0.0f, 100.0f
    );
    drawDisplay0();
    drawDisplay1();
}
void pos_callback(GLFWwindow* window, int x, int y) {
	drawDisplay0();
	drawDisplay1();
}