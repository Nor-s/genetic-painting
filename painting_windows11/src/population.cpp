#include "myheader/population.h"

namespace nsg
{
    GA::GA(int popSize, int dnaSize, int maxGen, float x, float y, float width, int height, float Sx, float Sy)
    {
        populationSize = popSize;
        dnaLen = dnaSize;
        currentGeneration = 0;
        maxGeneration = maxGen;
        originImageHeight = height;
        originImageWidth = width;
        initPopulation(x, y, width, height, Sx, Sy);
    }
    void GA::initPopulation(float x, float y, float width, int height, float Sx, float Sy)
    {
        for (int i = 0; i < populationSize; i++)
        {
            population.push_back(
                new DNA(
                    dnaLen,
                    x, y,
                    width, height,
                    Sx, Sy));
        }
    }
    void GA::pushBack(DNA *a)
    {
        population.push_back(a);
        populationSize++;
    }
    void GA::popBack()
    {
        if (population.size() == 0)
        {
            return;
        }
        DNA *tmp = population.back();
        population.pop_back();
        delete tmp;
        populationSize--;
    }
    void GA::drawDNA(int idx)
    {
        population[idx]->drawAll();
    }
    DNA *GA::top()
    {
        return population[0];
    }

    int GA::size()
    {
        return populationSize;
    }
    double GA::getFitness(int idx)
    {
        return population[idx]->fitnessRef();
    }

    void GA::sortDNA()
    {
        std::sort(population.begin(), population.end(), comp);
    }
    void GA::setCurrentPicture()
    {
    }
    void GA::nextGeneration()
    {
        int size = populationSize;
        for (int i = 0; i < size / 2; i++)
        {
            popBack();
        }
        for (int i = 0; i < size / 2; i++)
        {
            int parent1 = rand() % 3;
            int parent2 = rand() % 3;
            pushBack(population[parent1]->crossover(*population[parent2]));
            if (rand() % 10 == 0)
            {
                population.back()->mutate();
            }
        }
    }
}
bool comp(nsg::DNA *a, nsg::DNA *b)
{
    return a->fitnessRef() > b->fitnessRef();
}
/*
    using cosine similarity:  https://en.wikipedia.org/wiki/Cosine_similarity
*/
double fitnessFunction(GLubyte **a, GLubyte **b, int width, int height)
{
    double ret = 0.0;
    double dot = 0.0, denomA = 0.0, denomB = 0.0;
    for (int s = 0; s < 1; s++)
    {
        for (int i = 0; i < height / 2; i++)
        {
            for (int j = 0; j < width * 3; j += 3)
            {
                int k = i * width * 3 + j;
                dot += a[s][k] * b[s][k];
                denomA += a[s][k] * a[s][k];
                denomB += b[s][k] * b[s][k];
            }
        }
        ret = (dot / (sqrt(denomA) * sqrt(denomB)));
        denomA = dot = denomB = 0.0;
    }
    delete[] b;
    return ret;
}

namespace nsg
{
    void GA::set_picture_to_data(SquareObject *picture)
    {
        WindowControl::rendering_lock();
        GLFWwindow *tmpWindow = glfwGetCurrentContext();

        glfwMakeContextCurrent(WindowControl::g_windows_[0]->get_window());
        glDrawBuffer(GL_BACK);
        WindowControl::g_windows_[0]->window_clear_white();
        picture->draw();
        GLubyte **ret = WindowControl::g_windows_[1]->get_window_pixel();
        WindowControl::g_windows_[1]->screenshot(ret);

        glfwMakeContextCurrent(tmpWindow);
        WindowControl::rendering_unlock();
        grayscaledOriginPicture = ret;
    }
    void GA::caculateFitness()
    {
        for (int i = 0; i < populationSize; i++)
        {
            population[i]->fitnessRef() = fitnessFunction(
                grayscaledOriginPicture,
                population[i]->getPicture(),
                originImageWidth,
                originImageHeight);
        }
    }
}