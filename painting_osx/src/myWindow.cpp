#include "myheader/myWindow.h"

namespace nsg {    
    myWindow::myWindow() {
        SCR_WIDTH = 800;
        SCR_HEIGHT = 800;
        window = initWindow();
        initPBO();
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
    void myWindow::initPBO(){
        unsigned int bufferSize = SCR_WIDTH * SCR_HEIGHT * 3;

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
        glDrawBuffer(GL_FRONT);//(GL_BACK);

//if width and height resize => clear data and re assign

        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[0]);
        glReadPixels(
            0, 0,
            SCR_WIDTH, SCR_HEIGHT/2,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            0
        );

        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[1]);
        //프런트 버퍼로 부터 픽셀 정보들을 얻어온다.
    	glReadPixels(
    		0, SCR_HEIGHT/2,					    //캡처할 영역의 좌측상단 좌표
    		SCR_WIDTH, SCR_HEIGHT/2,		//캡처할 영역의 크기
    		GL_BGR,					    //캡처한 이미지의 픽셀 포맷
    		GL_UNSIGNED_BYTE,    		//캡처한 이미지의 데이터 포맷
            0
		);

// Process partial images.  Mapping the buffer waits for
// outstanding DMA transfers into the buffer to finish.
        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[0]);
        GLubyte* pboMem1 = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        //processImage(pboMem1);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[1]);
        GLubyte* pboMem2 = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
 //       processImage(pboMem2);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

/*
        //unmap the buffer
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo[0]);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo[0]);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
*/
std::cout<<"print image\n";
        
    }
    
    void windowResizeCallback(GLFWwindow* windowPointer, int width, int height) {
        glViewport(0, 0, width, height);
    }
}