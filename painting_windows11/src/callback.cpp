#include "myheader/callback.h"

namespace genetic_painting {
    void init_callback(GLFWwindow* window) {
        //callback
        glfwSetDropCallback(window, drop_callback);
        glfwSetFramebufferSizeCallback(window, resize_callback);
	    glfwSetWindowPosCallback(window, pos_callback);
    }

    void drop_callback(GLFWwindow* window, int count, const char** filepaths)
    {
        if(WindowControl::inputpath_ == "") {
            WindowControl::inputpath_ = std::string(filepaths[0]);
        #ifdef DEBUG_MODE
            std::cout<<"filename: "<<filepaths[0]<<"\n";
        #endif
        }
    }

    void resize_callback(GLFWwindow* windowPointer, int width, int height) {
        glViewport(0, 0, width, height);
        float width_f = static_cast<float>(WindowControl::global_width_);
        float height_f = static_cast<float>(WindowControl::global_height_);
        float widthFactor = static_cast<float>(width) / width_f;
        float heightFactor = static_cast<float>(height) / height_f;
        WindowControl::projection_transformation_ = glm::ortho(
             -width_f/2.0f * widthFactor, width_f/2.0f * widthFactor, 
             -height_f/2.0f * heightFactor, height_f/2.0f * heightFactor, 
            0.0f, 100.0f
        );

        WindowControl::rendering_0();
        WindowControl::rendering_1();
    }

    void pos_callback(GLFWwindow* window, int x, int y) {
       // for adjacent two window 
       int posx = 0, posy = 0;
        glfwGetWindowPos(WindowControl::v_window_[0]->get_window(), &posx, &posy);
        if(WindowControl::current_activate_window_num_ == 2) {
            glfwSetWindowPos(WindowControl::v_window_[1]->get_window(), posx + WindowControl::global_width_, posy);
        }
    
        WindowControl::rendering_0();
        WindowControl::rendering_1();;
    }
}