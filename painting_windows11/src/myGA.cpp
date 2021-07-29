#include "myheader/myGA.h"

/*

    DNA
    
*/
namespace nsg {
    DNA::DNA(DNA& a, DNA& b) {
        dnaSize = a.dnaSize;
        int pivot = rand()%(dnaSize-1) + 1;
        for(int i = 0; i < pivot; i++) {
            dna.push_back({i, a.dna[i].second});
            brushPointers.push_back({true, a.dna[i].second});
            a.brushPointers[i].first = false;
        }
        for(int i = pivot; i < dnaSize; i++) {
            dna.push_back({i, b.dna[i].second});
            brushPointers.push_back({true, b.dna[i].second});
            b.brushPointers[i].first = false;
        }
        fitness = 0;
    }

    DNA::DNA(int n, float x, float y, float width, int height, float Sx, float Sy) {
        dnaSize = n;
        initDNA(x, y, width, height, Sx, Sy);
    }
    DNA::~DNA(){
    #ifdef DEBUG_MODE
        std::cout<<"~~~DNA\n";
    #endif 
        std::cout<<"~~~DNA\n";
        for(int i = 0; i < dnaSize; i++) {
            if(brushPointers[i].first == true) {

                delete brushPointers[i].second;
            }
        }
    }
    void DNA::initDNA(float x, float y, float width, float height, float Sx, float Sy) {
        for(int i = 0; i < dnaSize; i++) {
            dna.push_back({i, new myPainting(x, y, width, height, Sx, Sy)});
            brushPointers.push_back({true, dna.back().second});
        }
    }
    
    bool DNA::operator <(DNA& a) {
        return fitness < a.fitness;
    }    
   
    void DNA::mutate(int n, float x, float y, float width, float height, float Sx, float Sy) {
        for(int i = 0; i < n; i++) {
            if(rand()%100 == 0) {
                dna[i].second->initTransform();
                dna[i].second->setTransformToRand(x, y, width, height, Sx, Sy);
            }
        }
    }
    DNA* DNA::crossover(DNA& a) {
        return new DNA(a, a);
    } 
    void DNA::draw(int i) {
        dna[i].second->setProjectionToUniform(myWindow::projection);
        dna[i].second->draw();
    }
    void DNA::drawAll() {
        for(int i = 0; i < dnaSize; i++) {
            this->draw(i);
        }
    }

    int DNA::size() {
        return dnaSize;
    }

    GLubyte** DNA::getPicture(){
        myWindow::drawingLock();
        glfwMakeContextCurrent(myWindow::windowDictionary[1]->getWindow());
        glDrawBuffer(GL_BACK);
        myWindow::windowDictionary[1]->windowClear(GL_COLOR_BUFFER_BIT, 1.0, 1.0, 1.0, 1.0);
        drawAll();
        GLubyte** ret = myWindow::windowDictionary[1]->getWindowPixel();
        myWindow::drawingUnLock();
        return ret;
    }
    float& DNA::fitnessRef() {
        return fitness;
    }

}
/*

    GA

*/
namespace nsg {
    GA::GA(int popSize, int dnaSize, int maxGen, float x, float y, float width, int height, float Sx, float Sy) {
        populationSize = popSize;
        dnaLen = dnaSize;
        maxGeneration = maxGen;

        initPopulation(x, y, width, height, Sx, Sy);
    }
    void GA::initPopulation(float x, float y, float width, int height, float Sx, float Sy) {
        for(int i = 0; i < populationSize; i++) {
            population.push_back(
                new DNA(
                dnaLen,
                x, y,
                width, height,
                Sx, Sy
                )
            );
        }
    }
    void GA::pushBack(DNA* a) {
        population.push_back(a);
    }
    void GA::popBack(){
        population.pop_back();
    }
    void GA::drawDNA(int idx) {
        population[idx]->drawAll();
    }
    DNA* GA::top(){
        return population.back();
    }
    void GA::caculateFitness() {
        for(int i = 0; i < populationSize; i++) {
            population[i]->fitnessRef()
             = fitnessFunction(
                 greyscaledOriginFile,
                 population[i]->getPicture()
                );
        }
    }
    
    void GA::sortDNA() {
        std::sort(population.begin(), population.end(), comp);
    }
}    
bool comp(nsg::DNA* a, nsg::DNA* b) {
    return a->fitnessRef() < b->fitnessRef();
}
float fitnessFunction(GLubyte** a, GLubyte** b) {
    int ret = rand()%10;
    //need delete b

    delete[] b;
    return (float)ret;
}