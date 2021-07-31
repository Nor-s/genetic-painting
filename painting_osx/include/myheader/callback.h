#ifndef CALLBACK_H
#define CALLBACK_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "myheader/window_control.h"
#include "myheader/genetic_algorithm.h"

#include <stdlib.h>
#include <time.h>
#include <vector>

namespace nsg
{
    void render_callback();
    void init_callback(GLFWwindow *window, bool is_drop);
    void drop_callback(GLFWwindow *window, int count, const char **paths);
    void resize_callback(GLFWwindow *window, int width, int height);
    void pos_callback(GLFWwindow *window, int x, int y);
}
#endif