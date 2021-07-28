#include "myHeader/myGA.h"

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
    }

    DNA::DNA(int n, float x, float y, float width, int height, float Sx, float Sy) {
        dnaSize = n;
        initDNA(x, y, width, height, Sx, Sy);
    }
    DNA::~DNA(){
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
        return similarity < a.similarity;
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
            draw(i);
        }
    }

    int DNA::size() {
        return dnaSize;
    }

    GLubyte** DNA::getPicture(){
        GLubyte** ret;
        return ret;
    }
}
/*

    GA

*/
namespace nsg {
    GA::GA(int n) {

    }
    void GA::crossOver() {

    }
    void GA::initPopulation() {

    }
    void GA::addPopulation() {

    }

    float GA::caculateSimilarity(DNA& dna1) {
        float ret;
        return ret;
    }

}