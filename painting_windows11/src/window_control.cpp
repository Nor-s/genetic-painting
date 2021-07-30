#include "myheader/window_control.h"

/*
    static memeber var
*/
namespace genetic_painting {    
    GLboolean WindowControl::semaphore_rendering_        = true;
    PictureObject* WindowControl::picture_               = nullptr;
    Population* WindowControl::population_  = nullptr;
    std::vector<WindowControl*> WindowControl::v_window_;
    glm::mat4 WindowControl::projection_transformation_;
    GLint WindowControl::current_activate_window_num_    = 0;
    GLint WindowControl::global_width_                   = 800;
    GLint WindowControl::global_height_                  = 800;
    std::string WindowControl::inputpath_                = "";
}

namespace genetic_painting {
    WindowControl::WindowControl(int width, int height, const char* title) {        
        stbi_flip_vertically_on_write(true);
        width_ = global_width_ = width;
        height_ = global_height_ = height;
        title_ = std::string(title);
        window_ = init_window_control();
        projection_transformation_ = glm::ortho(
            (float)-width/2.0f, (float)width/2.0f,
            (float)-height/2.0f, (float)height/2.0f,
            0.0f, 100.0f
        );
        glViewport(0.0, 0.0, width, height);
        v_window_.push_back(this);         
        init_pixel_buffer_object();
    }
    GLFWwindow* WindowControl::init_window_control() {

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        // glfw window creation
        // --------------------
        GLFWwindow* window = glfwCreateWindow(width_, height_, title_.c_str(), NULL, NULL);
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
    GLFWwindow* WindowControl::get_window() {
        return window_;
    }
    GLubyte** WindowControl::get_pixel_halfwindows() {
        GLubyte** pbomem = new GLubyte*[2];

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer_object_[0]);
        glReadPixels(
            0, 0,
            width_, height_/2,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            0
        );

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer_object_[1]);
    	glReadPixels(
    		0, height_/2,					    
    		width_, height_/2,		
    		GL_BGR,					    
    		GL_UNSIGNED_BYTE,    		
            0
		);

// Process partial images.  Mapping the buffer waits for
// outstanding DMA transfers into the buffer to finish.
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer_object_[0]);
        pbomem[0] = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer_object_[1]);
        pbomem[1] = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);



        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer_object_[0]);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer_object_[1]);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        return pbomem;
    }
    GLubyte* WindowControl::get_pixel_fullwindow() {

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer_object_[2]);
        glReadPixels(
            0, 0,
            width_, height_/2,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            0
        );

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer_object_[2]);
        GLubyte* pbomem = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        return pbomem;
    }
    void WindowControl::rendering_lock() {
        while(!semaphore_rendering_){
        }
        semaphore_rendering_ = false;
    }
    void WindowControl::rendering_unlock() {
        semaphore_rendering_ = true;
    }
    void WindowControl::set_windowsize(int width, int height) {
        width_  = width;
        height_ = height;
        glfwSetWindowSize(get_window(), width_, height_);
        glDeleteBuffers(3, pixel_buffer_object_);
        init_pixel_buffer_object();
    }
    void WindowControl::init_pixel_buffer_object(){
        glfwGetWindowSize(get_window(), &width_, &height_);
        unsigned int buffersize = width_ * (height_)/2 * 3;

        //pbo gen
        glGenBuffers(3, pixel_buffer_object_);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer_object_[0]);
        glBufferData(GL_PIXEL_PACK_BUFFER, buffersize/2, nullptr, GL_STREAM_READ);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer_object_[1]);
        glBufferData(GL_PIXEL_PACK_BUFFER, buffersize/2, nullptr, GL_STREAM_READ);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer_object_[2]);
        glBufferData(GL_PIXEL_PACK_BUFFER, buffersize*2, nullptr, GL_STREAM_READ);

        //unBind
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }
    void WindowControl::clear_window( GLfloat r, GLfloat g, GLfloat b, GLfloat w) {
        glClearColor(r, g, b, w);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    }

    void WindowControl::screenshot_png(const char *strFilePath) {
        int stridesize = width_*3; //+ width_%4;

        GLubyte* pbomem = get_pixel_fullwindow();
        stbi_write_png("screenshot.png", width_, height_, 3, pbomem, stridesize);
        //delete pbomem
    }
}



namespace genetic_painting {
    GLubyte** WindowControl::get_dna_image(int idx){
        rendering_lock();

        glfwMakeContextCurrent(v_window_[1]->get_window());
        glDrawBuffer(GL_BACK);
        v_window_[1]->clear_window( 1.0, 1.0, 1.0, 1.0);
        population_->draw_dna(idx, projection_transformation_);
        GLubyte** ret = v_window_[1]->get_pixel_halfwindows();

        rendering_unlock();
        return ret;
    }

}

namespace genetic_painting {
    void WindowControl::rendering_1() {
        if(WindowControl::current_activate_window_num_ == 2) {
            WindowControl::rendering_lock();
/*
            glfwMakeContextCurrent(v_window_[1]->get_window());
            v_window_[1]->clear_window(1.0f, 1.0f, 1.0f, 1.0f);
            if(population_ != nullptr) {
                population_->top()->drawAll();
                population_->caculateFitness();
                population_->sortDNA();
            #ifdef DEBUG_MODE
                std::cout<<"top: "<<population_->top()->fitnessRef()<<"\n";
                int population_Size = population_->size();
                for(int i = 0; i < population_Size; i++) {
                    std::cout<<i<<" : "<<population_->getFitness(i)<<"\n";
                }
                std::cout<<"-----\n";
            #endif
                population_->nextGeneration();
            }
            
*/
            glfwSwapBuffers(WindowControl::v_window_[1]->get_window());

            WindowControl::rendering_unlock();
        }
    }

    void WindowControl::rendering_0() {
        if(WindowControl::current_activate_window_num_ == 2) {
            rendering_lock();
            glfwMakeContextCurrent(WindowControl::v_window_[0]->get_window());
            v_window_[0]->clear_window(0.0f, 0.0f, 0.0f, 1.0f);
            picture_->set_projection_transform(projection_transformation_);
            picture_->draw();
            glfwSwapBuffers(WindowControl::v_window_[0]->get_window());

         rendering_unlock();
        }
    }

    /*
        using cosine similarity:  https://en.wikipedia.org/wiki/Cosine_similarity
    */
    float WindowControl::get_similarity(GLubyte** b, int width, int height) {
        GLubyte** a = grayscaled_origin_image_;
        float dot = 0.0f, denomA = 0.0f, denomB = 0.0f;
        for(int i = 0; i < width*3; i += 3) {
            for(int j = 0; j < height/2; j++) {
                int k = i + j*width*3;
                dot += (a[0][k]*b[0][k] + a[1][k]*b[1][k])*3;
                denomA += (a[0][k]*a[0][k] + a[1][k]*a[1][k])*3;
                denomB += (b[0][k]*b[0][k] + b[1][k]*b[1][k])*3;
            }
        }
        delete[] b;
        return dot/(sqrt(denomA)*sqrt(denomB));
    }
    void WindowControl::caculate_all_fitness(int width, int height) {
        int size = population_->get_population_size();
        for(int i = 0; i < size; i++) {
            population_->set_dna_fitness(i, get_similarity(get_dna_image(i), width, height));
        }
    }

}
