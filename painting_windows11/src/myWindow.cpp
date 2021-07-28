#include "myheader/myWindow.h"

namespace nsg {    
    int myWindow::SCR_HEIGHT;
    int myWindow::SCR_WIDTH;
    glm::mat4 myWindow::projection;

    myWindow::myWindow(int width, int height, const char* title) {
        stbi_flip_vertically_on_write(true);
        currentWidth = SCR_WIDTH = width;
        currentHeight = SCR_HEIGHT = height;
        window = initWindow(width, height, title);
        projection = glm::ortho(
            (float)-width/2.0f, (float)width/2.0f,
            (float)-height/2.0f, (float)height/2.0f,
            0.0f, 100.0f
        );
        initPBO();
    }
    
    GLFWwindow* myWindow::initWindow(int width, int height, const char* title) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        // glfw window creation
        // --------------------
        GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);
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
    #ifdef DEEPTH_TEST
        glClear(mask | GL_DEPTH_TEST);
    #else
        glClear(mask);
    #endif
    }

    void myWindow::windowCapture(const char *strFilePath) {
        glfwMakeContextCurrent(getWindow());
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
    		0, currentWidth/2,					    
    		currentWidth, currentHeight/2,		
    		GL_BGR,					    
    		GL_UNSIGNED_BYTE,    		
            0
		);

// Process partial images.  Mapping the buffer waits for
// outstanding DMA transfers into the buffer to finish.
        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[0]);
        GLubyte* pboMem1 = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        stbi_write_png("s11.png", currentWidth, currentHeight/2, 3, pboMem1, SCR_WIDTH*3);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[1]);
        GLubyte* pboMem2 = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        stbi_write_png("s111.png", currentWidth, currentHeight/2, 3, pboMem1, SCR_WIDTH*3);

     //   glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        //unmap the buffer
        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[0]);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[1]);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

        glDeleteBuffers(2, PBO);
    }

}