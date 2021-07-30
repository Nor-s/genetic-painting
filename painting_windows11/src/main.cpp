#define DEBUG_MODE
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "myheader/callback.h"
#include "myheader/window_control.h"
#include "myheader/population_dna.h"
#include "myheader/square_object2d.h"


using namespace genetic_painting;

inline void process_input(WindowControl* window);
inline void rendering_loop_until_drop(WindowControl* window);
inline void init_picture(WindowControl* window); 
inline void init_genetic_algorithm(WindowControl* window);
inline void rendering_loop_until_exit(WindowControl* window);


GLint global_population_size       = 20;
GLint global_dna_len               = 10;
GLint global_max_stage             = 100;

int main() {
    srand (static_cast <unsigned> (time(0)));  

    glfwInit();
    
    WindowControl* window = new WindowControl(300, 300, "Picture");      
    init_callback(window->get_window());
 //   glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    rendering_loop_until_drop(window);

    init_picture(window);
    


    init_genetic_algorithm(window);    
    rendering_loop_until_exit(window);

    glfwTerminate();
    return 0;
}

inline void rendering_loop_until_drop(WindowControl* window) {
    while(!glfwWindowShouldClose(window->v_window_[0]->get_window()) 
      && window->inputpath_ == "") {
        process_input(window);
        glfwPollEvents();
    }
}
inline void init_picture(WindowControl* window) {
#ifdef DEBUG_MODE
    std::cout<<"input:path: "<<WindowControl::inputpath_<<"\n";
#endif
    WindowControl::picture_ = new PictureObject(WindowControl::inputpath_);
    int posx, posy;
    int width  = WindowControl::picture_->get_texture_width();
    int height = WindowControl::picture_->get_texture_height();
    window->set_windowsize(width, height);
    glfwGetWindowPos(window->get_window(), &posx, &posy);

    WindowControl* second_window = new WindowControl(width, height, "painting");
    glfwSetWindowPos(second_window->get_window(), posx + width, posy);
    init_callback(second_window->get_window());

    window->rendering_0();
}
inline void init_genetic_algorithm(WindowControl* window) {
    window->population_ = new Population (
        global_population_size, global_dna_len, global_max_stage, 
        0.0f, 0.0f, 
        static_cast<float>(window->global_width_), 
        static_cast<float>(window->global_height_),
        1.0f, 1.0f
    );
    //setOriginPicture(window->picture_);
}
inline void rendering_loop_until_exit(WindowControl* window){
    while(!glfwWindowShouldClose(window->get_window())) {
        process_input(window);
        window->rendering_0();
        window->rendering_1();        
        glfwPollEvents();
    }
}
inline void process_input(WindowControl* window) {
        if (glfwGetKey(window->get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window->get_window(), true);
        }
        else if (window->current_activate_window_num_ == 2 
                 && glfwGetKey(window->get_window(), GLFW_KEY_1) == GLFW_PRESS) {
            window->rendering_lock();

            glDrawBuffer(GL_BACK);
            window->rendering_1();
            window->v_window_[1]->screenshot_png("sdf");
            
            window->rendering_unlock();
    }
}