#include "myheader/myGA.h"

/*

    DNA
    
*/
namespace nsg
{
    std::vector<SquareObject *> DNA::brushes_;
    DNA::DNA(DNA &a, DNA &b)
    {
        dnaSize = a.dnaSize;
        x_ = a.x_;
        y_ = a.y_;
        sx_ = a.sx_;
        width_ = a.width_;
        height_ = a.height_;
        sxx_ = a.sxx_;
        int pivot = rand() % (dnaSize - 2) + 1;
        for (int i = 0; i < pivot; i++)
        {
            dna.push_back(a.dna[i]);
        }
        for (int i = pivot; i < dnaSize; i++)
        {
            dna.push_back(b.dna[i]);
        }
        fitness = 0;
    }

    DNA::DNA(int n, float x, float y, float width, int height, float Sx, float Sy)
    {
        if (brushes_.size() == 0)
        {
            brushes_.push_back(new SquareObject(0));
            brushes_.push_back(new SquareObject(1));
            brushes_.push_back(new SquareObject(2));
            brushes_.push_back(new SquareObject(3));
        }
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;
        sx_ = Sx;
        sxx_ = Sy;
        dnaSize = n;
        initDNA();
    }
    DNA::~DNA()
    {
    }
    void DNA::initDNA()
    {
        for (int i = 0; i < dnaSize; i++)
        {
            dna.push_back(Brush());
            set_rotate(dna.back());
            set_brightness(dna.back());
            set_brushidx(dna.back());
            set_scale(dna.back());
            set_translate(dna.back());
        }
    }

    bool DNA::operator<(DNA &a)
    {
        return fitness < a.fitness;
    }
    void DNA::set_rotate(Brush &brush)
    {
        brush.rotate = getRandFloat(0.0f, 360.0f);
    }
    void DNA::set_brightness(Brush &brush)
    {
        brush.brightness = getRandFloat(0.0f, 1.0f);
    }
    void DNA::set_brushidx(Brush &brush)
    {
        brush.idx = rand() % brushes_.size();
    }
    void DNA::set_scale(Brush &brush)
    {
        brush.scale = getRandFloat(sx_, sxx_);
    }
    void DNA::set_translate(Brush &brush)
    {
        brush.translate[0] = getRandFloat(-width_ / 2.0f + x_, width_ / 2.0f + x_);
        brush.translate[1] = getRandFloat(-height_ / 2.0f + y_, height_ / 2.0f + y_);
        brush.translate[2] = -getRandFloat(2.0f, 50.0f);
    }
    void DNA::mutate()
    {
        for (int i = 0; i < dnaSize; i++)
        {
            if (rand() % 10 == 0)
            {
                if (rand() % 2 == 0)
                {
                    set_rotate(dna[i]);
                }
                if (rand() % 2 == 0)
                {
                    set_brightness(dna[i]);
                }
                if (rand() % 2 == 0)
                {
                    set_brushidx(dna[i]);
                }
                if (rand() % 2 == 0)
                {
                    set_scale(dna[i]);
                }
                if (rand() % 2 == 0)
                {
                    set_translate(dna[i]);
                }
            }
        }
    }
    DNA *DNA::crossover(DNA &a)
    {
        return new DNA(a, a);
    }
    void DNA::draw(int i)
    {
        SquareObject *p = brushes_[dna[i].idx];
        p->init_model();
        p->translate(dna[i].translate);
        p->rotate(dna[i].rotate);
        p->scale(dna[i].scale);
        p->scale(0.4);
        p->set_bright_to_uniform(dna[i].brightness);
        p->set_model_to_uniform();
        p->draw();
    }
    void DNA::drawAll()
    {
        for (int i = 0; i < dnaSize; i++)
        {
            this->draw(i);
        }
    }

    int DNA::size()
    {
        return dnaSize;
    }

    double &DNA::fitnessRef()
    {
        return fitness;
    }

}
/*

    GA

*/
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
float getRandFloat(float lo, float hi)
{
    return lo + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (hi - lo)));
}

namespace nsg
{
    GLubyte **DNA::getPicture()
    {
        WindowControl::rendering_lock();
        glfwMakeContextCurrent(WindowControl::g_windows_[1]->get_window());
        glDrawBuffer(GL_BACK);
        WindowControl::g_windows_[1]->window_clear_white();
        //drawing back()
        drawAll();
        GLubyte **ret = WindowControl::g_windows_[1]->get_window_pixel();
        WindowControl::rendering_unlock();
        return ret;
    }
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