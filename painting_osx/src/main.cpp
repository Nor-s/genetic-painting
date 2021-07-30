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

void rendering_0();
void rendering_1();
void processInput();

GA *genetic_algo_ = nullptr;

int populationSize = 150;
int dnaLen = 100;
int maxGeneration = 100;

int main()
{
    srand(static_cast<unsigned>(time(0)));

    glfwInit();
    WindowControl::g_windows_.push_back(new WindowControl(300, 300, "Picture"));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG_MODE
    std::cout << "window setup\n";
#endif
    init_callback(WindowControl::g_windows_[0]->get_window());

#ifdef DEBUG_MODE
    std::cout << "callback setup\n";
#endif

    //for drop image
    while (WindowControl::size_ == 1 && !glfwWindowShouldClose(WindowControl::g_windows_[0]->get_window()))
    {
        processInput();
        glfwPollEvents();
    }

#ifdef DEBUG_MODE
    std::cout << "second loop start\n";
    std::cout << "population size:" << populationSize << " , dnaLen: " << dnaLen << " maxGeneration: " << maxGeneration << "\n";
#endif
    glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
    if (genetic_algo_ == nullptr)
    {
        genetic_algo_ = new GA(
            populationSize, dnaLen, maxGeneration,
            0.0f, 0.0f,
            WindowControl::g_width_, WindowControl::g_height_,
            0.1f, 0.2f);
        genetic_algo_->set_picture_to_data(WindowControl::picture_);
    }
    //for startWindowControl::genetic_algo_
    while (!glfwWindowShouldClose(WindowControl::g_windows_[0]->get_window()))
    {
        processInput();
        rendering_0();
        rendering_1();

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput()
{
    if (glfwGetKey(WindowControl::g_windows_[0]->get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(WindowControl::g_windows_[0]->get_window(), true);
    }
    else if (WindowControl::size_ == 2 && glfwGetKey(WindowControl::g_windows_[1]->get_window(), GLFW_KEY_1) == GLFW_PRESS)
    {
        WindowControl::rendering_lock();

        glDrawBuffer(GL_BACK);
        WindowControl::g_windows_[1]->window_clear_white();
        rendering_1();
        WindowControl::g_windows_[1]->window_capture("sdf");

        WindowControl::rendering_unlock();
    }
}

void rendering_1()
{
    if (WindowControl::size_ == 2)
    {
        WindowControl::rendering_lock();
        glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
        WindowControl::g_windows_[1]->window_clear_white();

        if (genetic_algo_ != nullptr)
        {
            genetic_algo_->caculateFitness();
            genetic_algo_->sortDNA();
            WindowControl::g_windows_[1]->window_clear_white();
            genetic_algo_->top()->drawAll();
#ifdef DEBUG_MODE
            std::cout << "top: " << genetic_algo_->top()->fitnessRef() << "\n";
            int genetic_algo_Size = genetic_algo_->size();
            for (int i = 0; i < genetic_algo_Size; i++)
            {
                std::cout << i << " : " << genetic_algo_->getFitness(i) << "\n";
            }
            std::cout << "-----\n";
#endif
            genetic_algo_->nextGeneration();
        }
        glfwSwapBuffers(WindowControl::g_windows_[1]->get_window());
        WindowControl::rendering_unlock();
    }
}
void rendering_0()
{
    if (WindowControl::size_ == 2)
    {
        WindowControl::rendering_lock();
        glfwMakeContextCurrent(WindowControl::g_windows_[0]->get_window());

        WindowControl::g_windows_[0]->window_clear_white();
        WindowControl::picture_->draw();
        glfwSwapBuffers(WindowControl::g_windows_[0]->get_window());

        WindowControl::rendering_unlock();
    }
}
