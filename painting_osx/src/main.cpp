#define STB_IMAGE_IMPLEMENTATION
#include "myheader/myWindow.h"
#include "myheader/myPainting.h"
//#include "myheader/myCallback.h"

int main() {
    glfwInit();
    nsg::myWindow window;
    nsg::myPainting brush;

    while(!glfwWindowShouldClose(window.getWindow())) {
        nsg::processInput(window.getWindow());

        window.windowClear(GL_COLOR_BUFFER_BIT, 0.2f, 0.2f, 0.0f, 1.0f);

        brush.draw();

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}