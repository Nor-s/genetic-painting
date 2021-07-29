#ifndef MY_GA_H
#define MY_GA_H

#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <stdlib.h>

#include "myheader/myWindow.h"
#include "myheader/myPainting.h"
/*
            dna.first     : individual similarity (later)
            dna.second    : chromosome : brush : pos, size, angle, brush type, brightness
            brushPointers : for the Destructor : for free pointer 
            
            crossover     : child just point their chromosome, one crossover
            mutation      : reset pos, size, angle, brush type, brightness (current possiblity = 1%)
            draw          : using PBO, draw back or front buffer 
            getPicture    : buffer data => GLubyte  : (0~ height/2, heihgt/2 ~ height)           
            similarity    : GA class measures similarity using getPicture. and return to DNA for sorting
*/
namespace nsg {
    class DNA {        
    private:
        std::vector<std::pair<float, myPainting*> > dna;
        std::vector<std::pair<bool, myPainting*>> brushPointers;

        float fitness;
        int dnaSize;
    public:  

        DNA(DNA& a, DNA& b);
        DNA(int n, float x, float y, float width, int height, float Sx, float Sy);
        ~DNA();

        bool operator <(DNA& a);

        void initDNA(float x, float y, float width, float height, float Sx, float Sy);

        void mutate(int n, float x, float y, float width, float height, float Sx, float Sy);
        DNA* crossover(DNA& a);
        void draw(int i);    
        void drawAll();
        int size();
        float& fitnessRef();

        GLubyte** getPicture();
    };

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
        void caculateFitness();
        void sortDNA();
        void setOriginPicture(myPainting* picture);
        int size();
        float getFitness(int idx);
        void nextGeneration();
        void setCurrentPicture();

    };
}
bool comp(nsg::DNA* a, nsg::DNA* b);
float fitnessFunction(GLubyte** a, GLubyte** b, int width, int height);

#endif