#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <time.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "myheader/callback.h"

using namespace nsg;

GeneticAlgorithm *g_genetic_manager;
/*
    window size control : square_object.cpp => init_texture => adjust tex_width, tex_height
    if macOS and using other display (!retina display)
    => set SquareObject => set_width , set_height , relative_width *= 1.0f
*/
std::pair<float, float> g_brush_width = {0.02, 0.15};
int g_population_size = 10;
int g_dna_len = 4;
int g_max_stage = 1;
int g_population_num = 1;

int main()
{
    /*
        GL_RGB  : picture_bpp = 3
        GL_RGBA : picture_bpp = 4
    */
    GeneticAlgorithm::picture_byte_per_pixel_ = 3;
    SquareObject::g_is_grayscale_ = false;

    srand(static_cast<unsigned>(time(0)));

    glfwInit();
    WindowControl::g_windows_.push_back(new WindowControl(300, 300, "Picture"));

    init_callback(WindowControl::g_windows_[0]->get_window(), true);

    g_genetic_manager = new GeneticAlgorithm(
        g_population_size,
        g_dna_len,
        g_max_stage,
        g_population_num,
        g_brush_width);

    g_genetic_manager->loop_until_drop();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    g_genetic_manager->init_population();

    g_genetic_manager->start_main_loop();

    glfwTerminate();
    return 0;
}