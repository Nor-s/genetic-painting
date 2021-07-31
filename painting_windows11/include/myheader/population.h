#ifndef MY_DNA_H
#define MY_DNA_H

#include "myheader/dna.h"

namespace nsg {
    class GA {
    private:
        std::vector<DNA*> population;
        GLubyte** grayscaledOriginPicture;
        GLubyte** currentPicture;

        int dnaLen;
        int populationSize;
        int maxGeneration;
        int currentGeneration;
        int originImageWidth, originImageHeight;
    public:
        GA(int popSize, int dnaSize, int maxGen, float x, float y, float width, int height, float Sx, float Sy);
        ~GA();
        void crossOver();
        void initPopulation(float x, float y, float width, int height, float Sx, float Sy);
        void pushBack(DNA* a);
        void popBack();
        DNA* top();
        void drawDNA(int idx);
        void sortDNA();
        int size();
        double getFitness(int idx);
        void nextGeneration();
        void setCurrentPicture();

        void set_picture_to_data(SquareObject* picture);
        void caculateFitness();

    };
}
bool comp(nsg::DNA* a, nsg::DNA* b);
double fitnessFunction(GLubyte** a, GLubyte** b, int width, int height);

#endif