#include <stdlib.h>
#include <time.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "myheader/myWindow.h"
#include "myheader/myPainting.h"

std::vector<nsg::myPainting*> canvas;

int main() {
    srand (static_cast <unsigned> (time(0)));  
    glfwInit();
    nsg::myWindow window;

    while(!glfwWindowShouldClose(window.getWindow())) {
        nsg::processInput(window.getWindow(), canvas);

        window.windowClear(GL_COLOR_BUFFER_BIT, 0.0f, 0.0f, 0.0f, 1.0f);

        for(int i = 0; i < static_cast<int>(canvas.size()); i++) {
            canvas[i]->draw();
        }

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}