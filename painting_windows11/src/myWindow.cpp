#include "myheader/myWindow.h"

namespace nsg {    
    int myWindow::SCR_HEIGHT;
    int myWindow::SCR_WIDTH;
    glm::mat4 myWindow::projection;

    myWindow::myWindow(int width, int height) {
        stbi_flip_vertically_on_write(true);
        width = SCR_WIDTH = 800;
        height = SCR_HEIGHT = 800;
        window = initWindow(width, height);
        glViewport(0,0,width,height);
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        initPBO();
    }
    GLFWwindow* myWindow::initWindow(int width, int height) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        // glfw window creation
        // --------------------
        GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
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
    void myWindow::initPBO(){
        glfwGetWindowSize(getWindow(), &currentWidth, &currentHeight);
        unsigned int bufferSize = currentWidth * currentHeight * 3;

//pbo gen
        glGenBuffers(2, PBO);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[0]);
        glBufferData(GL_PIXEL_PACK_BUFFER, bufferSize, nullptr, GL_STREAM_READ);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[1]);
        glBufferData(GL_PIXEL_PACK_BUFFER, bufferSize, nullptr, GL_STREAM_READ);

//unBind
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }
    void myWindow::windowClear(GLbitfield mask, GLfloat r, GLfloat g, GLfloat b, GLfloat w) {
        glClearColor(r, g, b, w);
        glClear(mask);
    }
    void myWindow::windowCapture(const char *strFilePath) {
        initPBO();
        glDrawBuffer(GL_FRONT);//(GL_BACK);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[0]);
        glReadPixels(
            0, 0,
            currentWidth, currentHeight/2,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            0
        );

        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[1]);
    	glReadPixels(
    		0, SCR_HEIGHT/2,					    
    		currentWidth, currentHeight/2,		
    		GL_BGR,					    
    		GL_UNSIGNED_BYTE,    		
            0
		);

// Process partial images.  Mapping the buffer waits for
// outstanding DMA transfers into the buffer to finish.
        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[0]);
        GLubyte* pboMem1 = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        stbi_write_png("sdf.png", SCR_WIDTH, SCR_HEIGHT/2, 3, pboMem1, SCR_WIDTH*3);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[1]);
        GLubyte* pboMem2 = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        stbi_write_png("sdf1.png", SCR_WIDTH, SCR_HEIGHT/2, 3, pboMem1, SCR_WIDTH*3);

     //   glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        //unmap the buffer
        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[0]);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[1]);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

        glDeleteBuffers(2, PBO);
    }
    
    void windowResizeCallback(GLFWwindow* windowPointer, int width, int height) {

        glViewport(0, 0, width, height);
        float widthFactor = static_cast<float>(width) / static_cast<float>(myWindow::SCR_WIDTH);
        float heightFactor = static_cast<float>(height) / static_cast<float>(myWindow::SCR_HEIGHT);

        myWindow::projection = glm::ortho(
            -1.0f * widthFactor, 
            1.0f * widthFactor, 
            -1.0f * heightFactor,
            1.0f * heightFactor, 
            -1.0f, 
            1.0f
        );
    }
}