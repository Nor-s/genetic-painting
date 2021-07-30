#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <time.h>
#include <vector>
#define DEBUG_MODE
#define SEMAPHORE_TEST

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "myheader/myGA.h"
#include "myheader/callback.h"
#include "myheader/myPainting.h"

using namespace nsg;

GeneticAlgorithm *g_genetic_manager;

int g_population_size = 100;
int g_dna_len         = 50;
int g_max_stage       = 100;

int main()
{
    srand(static_cast<unsigned>(time(0)));

    glfwInit();
    WindowControl::g_windows_.push_back(new WindowControl(300, 300, "Picture"));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    init_callback(WindowControl::g_windows_[0]->get_window());

    g_genetic_manager = new GeneticAlgorithm(
        g_population_size,
        g_dna_len,
        g_max_stage
    );

    g_genetic_manager->loop_until_drop();
            /*
            Shader::projection_matrix = glm::ortho(
            (float)-width / 2.0f, (float)width / 2.0f,
            (float)-height / 2.0f, (float)height / 2.0f,
            0.0f, 100.0f)
            */
    g_genetic_manager->init_population();
    g_genetic_manager->start_main_loop();

    glfwTerminate();
    return 0;
}