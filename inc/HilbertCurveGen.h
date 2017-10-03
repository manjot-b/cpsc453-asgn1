#ifndef HILBERT_CURVE_GEN
#define HILBERT_CURVE_GEN

#include <vector>
#include <glm/glm.hpp>


class HilbertCurveGen
{
    public: 
        HilbertCurveGen(int level);
        std::vector<float> getVerticies();
        void increaseLevel();
    private:
        std::vector<glm::vec4> verticies;
        void calcCurve(int level);
        void hilbert(float x0, float y0, float xis, float xjs, float yis, float yjs, int n);
        int level;
        
};

#endif