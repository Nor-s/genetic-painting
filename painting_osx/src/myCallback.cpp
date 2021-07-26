#include "myheader/myWindow.h"

namespace nsg {
    
    void processInput(GLFWwindow *windowPointer, std::vector<myPainting*> & canvas) {
        static int count = 1;
        if (glfwGetKey(windowPointer, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(windowPointer, true);
        }
        if (glfwGetKey(windowPointer, GLFW_KEY_A) == GLFW_PRESS) {
      //      if(count == 0) {
      //          return;
      //      }
       //     count--;
            canvas.push_back(new myPainting());
        }
        if (glfwGetKey(windowPointer, GLFW_KEY_S) == GLFW_PRESS) {
            if(count == 0) {
                count = 1;
            }
        }
    }
    
    void windowResizeCallback(GLFWwindow* windowPointer, int width, int height) {
        glViewport(0, 0, width, height);
    }
}