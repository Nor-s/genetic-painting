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
        //drawing back()
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
        currentGeneration = 0;
        maxGeneration = maxGen;
        originImageHeight = myWindow::SCR_HEIGHT;
        originImageWidth = myWindow::SCR_WIDTH;
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
        populationSize++;
    }
    void GA::popBack(){
        population.pop_back();
        populationSize--;
    }
    void GA::drawDNA(int idx) {
        population[idx]->drawAll();
    }
    void GA::setOriginPicture(myPainting* picture){
        myWindow::drawingLock();
        GLFWwindow* tmpWindow = glfwGetCurrentContext();

        glfwMakeContextCurrent(myWindow::windowDictionary[0]->getWindow());
        glDrawBuffer(GL_BACK);
        myWindow::windowDictionary[0]->windowClear(GL_COLOR_BUFFER_BIT, 1.0, 1.0, 1.0, 1.0);
        picture->setProjectionToUniform(myWindow::projection);
        picture->draw();
        GLubyte** ret = myWindow::windowDictionary[1]->getWindowPixel();

        glfwMakeContextCurrent(tmpWindow);
        myWindow::drawingUnLock();
        grayscaledOriginPicture = ret;
    }
    DNA* GA::top(){
        return population.back();
    }
    void GA::caculateFitness() {
        for(int i = 0; i < populationSize; i++) {

            population[i]->fitnessRef()
             = fitnessFunction(
                 grayscaledOriginPicture,
                 population[i]->getPicture(),
                 originImageWidth,
                 originImageHeight
                );
        }
    }
    int GA::size() {
        return populationSize;
    }
    float GA::getFitness(int idx) {
        return population[idx]->fitnessRef();
    }

    void GA::sortDNA() {
        std::sort(population.begin(), population.end(), comp);
    }
    void GA::setCurrentPicture() {

    }
    void GA::nextGeneration() {
        
    }
}    
bool comp(nsg::DNA* a, nsg::DNA* b) {
    return a->fitnessRef() < b->fitnessRef();
}
/*
    using cosine similarity:  https://en.wikipedia.org/wiki/Cosine_similarity
*/
float fitnessFunction(GLubyte** a, GLubyte** b, int width, int height) {
    float dot = 0.0f, denomA = 0.0f, denomB = 0.0f;
    for(int i = 0; i < width*3; i += 3) {
        for(int j = 0; j < height/2; j++) {
            int k = i + j*width*3;
            dot += (a[0][k]*b[0][k] + a[1][k]*b[1][k]);
            denomA += (a[0][k]*a[0][k] + a[1][k]*a[1][k]);
            denomB += (b[0][k]*b[0][k] + b[1][k]*b[1][k]);
        }
    }
    delete[] b;
    return dot/(sqrt(denomA)*sqrt(denomB));
}