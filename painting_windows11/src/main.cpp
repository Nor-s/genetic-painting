#include <stdlib.h>
#include <time.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "myheader/myWindow.h"
#include "myheader/myPainting.h"

void drawDisplay();
void processInput1(nsg::myWindow* window, std::vector<nsg::myPainting*>& canvas);
void drop_callback(GLFWwindow* window, int count, const char** paths);
void windowResizeCallback(GLFWwindow* windowPointer, int width, int height);
void pos_callback(GLFWwindow* window, int x, int y);

std::vector<nsg::myPainting*> canvas;
std::vector<nsg::myWindow*> windows;

int main() {
    srand (static_cast <unsigned> (time(0)));  

    glfwInit();
    windows.push_back(new nsg::myWindow(800, 800, "painting"));

    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    //callback
    glfwSetDropCallback(windows[0]->getWindow(), drop_callback);
    glfwSetFramebufferSizeCallback(windows[0]->getWindow(), windowResizeCallback);
	glfwSetWindowPosCallback(windows[0]->getWindow(), pos_callback);

    while(!glfwWindowShouldClose(windows[0]->getWindow())) {
        processInput1(windows[0], canvas);
        drawDisplay();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void drawDisplay(){
    windows[0]->windowClear(GL_COLOR_BUFFER_BIT, 0.0f, 0.0f, 0.0f, 1.0f);

    for(int i = 0; i < static_cast<int>(canvas.size()); i++) {
        canvas[i]->setProjectionToUniform(windows[0]->projection);
        canvas[i]->draw();
    }
    if(canvas.size() < 500) {
        canvas.push_back(new nsg::myPainting());
    }

    glfwSwapBuffers(windows[0]->getWindow());

    if(windows.size() != 1) {
        glfwMakeContextCurrent(windows[1]->getWindow());
        windows[1]->windowClear(GL_COLOR_BUFFER_BIT, 0.0f, 0.0f, 0.0f, 1.0f);
        glfwSwapBuffers(windows[1]->getWindow());
    }
    glfwMakeContextCurrent(windows[0]->getWindow());
}

void processInput1(nsg::myWindow *window, std::vector<nsg::myPainting*>& canvas) {
    static int count = 1;
    if (glfwGetKey(window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window->getWindow(), true);
    }
    if (glfwGetKey(window->getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        //canvas.push_back(new nsg::myPainting());
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

void drop_callback(GLFWwindow* window, int count, const char** paths)
{
    if(windows.size() == 2) {
        return;
    }
    int i;
    for (i = 0;  i < 1;  i++) {
        windows.push_back(new nsg::myWindow(800, 800, "picture"));
        glfwSetFramebufferSizeCallback(windows[1]->getWindow(), windowResizeCallback);
	    glfwSetWindowPosCallback(windows[1]->getWindow(), pos_callback);
        glfwMakeContextCurrent(windows[0]->getWindow());
        //handle_dropped_file(paths[i]);
    }
}

void windowResizeCallback(GLFWwindow* windowPointer, int width, int height) {

    glViewport(0, 0, width, height);
    float widthFactor = static_cast<float>(width) / static_cast<float>(nsg::myWindow::SCR_WIDTH);
    float heightFactor = static_cast<float>(height) / static_cast<float>(nsg::myWindow::SCR_HEIGHT);

    nsg::myWindow::projection = glm::ortho(
        -1.0f * widthFactor, 
        1.0f * widthFactor, 
        -1.0f * heightFactor,
        1.0f * heightFactor, 
        -1.0f, 
        1.0f
    );
    drawDisplay();
}
void pos_callback(GLFWwindow* window, int x, int y) {
	drawDisplay();
}