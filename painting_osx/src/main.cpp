#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <time.h>
#include <vector>

#define DEPTH_TEST
//#define DEBUG_MODE
#define SEMAPHORE_TEST

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "myheader/myWindow.h"
#include "myheader/myPainting.h"
#include "myheader/myGA.h"

void drawDisplay0();
void drawDisplay1();
void processInput();
void drop_callback(GLFWwindow* window, int count, const char** paths);
void windowResizeCallback(GLFWwindow* windowPointer, int width, int height);
void pos_callback(GLFWwindow* window, int x, int y);

std::vector<nsg::myWindow*> windows;
nsg::myPainting* picture = nullptr;
nsg::GA* ga = nullptr;

int main() {
    srand (static_cast <unsigned> (time(0)));  

    glfwInit();
    windows.push_back(new nsg::myWindow(300, 300, "Picture"));
    
#ifdef DEPPTH_TEST
    glEnable(GL_DEPTH_TEST);
#endif
    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    //callback
    glfwSetDropCallback(windows[0]->getWindow(), drop_callback);
    glfwSetFramebufferSizeCallback(windows[0]->getWindow(), windowResizeCallback);
	glfwSetWindowPosCallback(windows[0]->getWindow(), pos_callback);
    //for drop image
    while(windows.size() == 1&&!glfwWindowShouldClose(windows[0]->getWindow())) {
        processInput();
        glfwPollEvents();
    }
    std::cout<<"second loop start\n";

#ifdef DEBUG_MODE

#else
    glfwMakeContextCurrent(windows[1]->getWindow());
    if(ga == nullptr) {
        ga = new nsg::GA (
            10, 5, 100, 
            0.0f, 0.0f, 
            windows[0]->SCR_WIDTH, windows[0]->SCR_HEIGHT,
            1.0f, 1.0f
        );
    }

#endif

    //for startGA
    while(!glfwWindowShouldClose(windows[0]->getWindow())) {
        processInput();
        drawDisplay0();
        drawDisplay1();        


        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void drawDisplay1(){
    if(windows.size() == 2) {
        nsg::myWindow::drawingLock();
        glfwMakeContextCurrent(windows[1]->getWindow());
        windows[1]->windowClear(GL_COLOR_BUFFER_BIT, 1.0f, 1.0f, 1.0f, 1.0f);

        if(ga != nullptr) {
            ga->drawDNA(rand()%10);
        }
        glfwSwapBuffers(windows[1]->getWindow());
        nsg::myWindow::drawingUnLock();
    }
}

void drawDisplay0() {
    if(windows.size() == 2) {
        nsg::myWindow::drawingLock();
        glfwMakeContextCurrent(windows[0]->getWindow());

       windows[0]->windowClear(GL_COLOR_BUFFER_BIT, 0.0f, 0.2f, 0.2f, 1.0f);
        picture->setProjectionToUniform(nsg::myWindow::projection);
        picture->draw();
        glfwSwapBuffers(windows[0]->getWindow());

        nsg::myWindow::drawingUnLock();
    }
}

void processInput() {
    if (glfwGetKey(windows[0]->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(windows[0]->getWindow(), true);
    }
    else if (windows.size() == 2 && glfwGetKey(windows[1]->getWindow(), GLFW_KEY_1) == GLFW_PRESS) {
        nsg::myWindow::drawingLock();
        
        
        glDrawBuffer(GL_BACK);
        drawDisplay1();
        windows[1]->windowCapture("sdf");
        
        nsg::myWindow::drawingUnLock();
    }
}

void drop_callback(GLFWwindow* window, int count, const char** paths)
{
    if(windows.size() == 2) {
        return;
    }
    int i;
    for (i = 0;  i < 1;  i++) {
        nsg::myWindow::drawingLock();

        picture = new nsg::myPainting(paths[i]);
        int posx, posy;
        int width = picture->texWidth, height = picture->texHeight;

        glfwSetWindowSize(windows[0]->getWindow(), width, height);
        glfwGetWindowPos(windows[0]->getWindow(), &posx, &posy);


        windows.push_back(new nsg::myWindow(width, height, "painting"));
        glfwSetWindowPos(windows[1]->getWindow(), posx+width, posy);
        glfwSetFramebufferSizeCallback(windows[1]->getWindow(), windowResizeCallback);
	    glfwSetWindowPosCallback(windows[1]->getWindow(), pos_callback);

        nsg::myWindow::drawingUnLock();

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
        -100.0f, 100.0f
    );

    drawDisplay0();
    drawDisplay1();
}

void pos_callback(GLFWwindow* window, int x, int y) {
    // for adjacent two window 
    int posx = 0, posy = 0;
    glfwGetWindowPos(windows[0]->getWindow(), &posx, &posy);
    if(windows.size() == 2) {
        glfwSetWindowPos(windows[1]->getWindow(), posx + windows[0]->SCR_WIDTH, posy);
    }
    
	drawDisplay0();
	drawDisplay1();
}