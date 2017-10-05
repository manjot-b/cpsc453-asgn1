#ifndef HILBERT_CURVE_GEN
#define HILBERT_CURVE_GEN

#include <vector>
#include <glm/glm.hpp>


class HilbertCurveGen
{
    public: 
        HilbertCurveGen(int level);
        std::vector<float> getPointVerticies();
        std::vector<float> getTriangleVerticies();
        void increaseLevel();
        void decreaseLevel();
    private:
        int level;        
        std::vector<glm::vec4> verticies;
        glm::mat4 lowerLeftTrans;
        glm::mat4 upperLeftTrans;
        glm::mat4 upperRightTrans;
        glm::mat4 lowerRightTrans;
        
        void calcCurve(int level);
        void hilbert(std::vector<glm::vec4> &verticies, int n);
        bool isOrthogonal(glm::vec3 x, glm::vec3 y);

        
};

#endif