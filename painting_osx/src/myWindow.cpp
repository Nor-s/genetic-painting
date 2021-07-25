#include "myheader/myWindow.h"

namespace nsg {    
    myWindow::myWindow() {
        SCR_WIDTH = 800;
        SCR_HEIGHT = 800;
        window = initWindow();
    }
    GLFWwindow* myWindow::initWindow() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        // glfw window creation
        // --------------------
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, windowResizeCallback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return nullptr;
        }
        return window;
    }
    GLFWwindow* myWindow::getWindow() {
        return window;
    }
    void myWindow::windowClear(GLbitfield mask, GLfloat r, GLfloat g, GLfloat b, GLfloat w) {
        glClearColor(r, g, b, w);
        glClear(mask);
    }
}