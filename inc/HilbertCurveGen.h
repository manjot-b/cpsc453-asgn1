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
        void hilbert(std::vector<glm::vec4> &verticies, int n);
        int level;
        glm::mat4 lowerLeftTrans;
        glm::mat4 upperLeftTrans;
        glm::mat4 upperRightTrans;
        glm::mat4 lowerRightTrans;
        
};

#endif