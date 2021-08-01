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

std::pair<float, float> g_brush_width = {0.05, 0.08};
int g_population_size = 20;
int g_dna_len = 10;
int g_max_stage = 100000;

int main()
{
    srand(static_cast<unsigned>(time(0)));

    glfwInit();
    WindowControl::g_windows_.push_back(new WindowControl(300, 300, "Picture"));
        glEnable(GL_DEPTH_TEST);

    init_callback(WindowControl::g_windows_[0]->get_window(), true);

    g_genetic_manager = new GeneticAlgorithm(
        g_population_size,
        g_dna_len,
        g_max_stage,
        g_brush_width);

    g_genetic_manager->loop_until_drop();
        glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    g_genetic_manager->init_population();

    g_genetic_manager->start_main_loop();

    glfwTerminate();
    return 0;
}