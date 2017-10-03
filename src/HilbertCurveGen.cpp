#include "HilbertCurveGen.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

using namespace glm;
using namespace std;

HilbertCurveGen::HilbertCurveGen(int _level) :level(_level) 
{
    hilbert(-1.0f, -1.0f, 2.0f, 0.0f, 0.0f, 2.0f, level);
}

vector<float> HilbertCurveGen::getVerticies()
{
    vector<float> vert;
    for (const auto & vec : verticies)
    {
        vert.push_back(vec.x);
        vert.push_back(vec.y);
        vert.push_back(vec.z);
    }
    return vert;
}

void HilbertCurveGen::increaseLevel()
{
    verticies.clear();
    hilbert(-1.0f, -1.0f, 2.0f, 0.0f, 0.0f, 2.0f, ++level);
}

/* x0 and y0 are the coordinates of the bottom left corner */
/* xis & xjs are the i & j components of the unit x vector this frame */
/* similarly yis and yjs */
void HilbertCurveGen::hilbert(float x0, float y0, float xis, float xjs, float yis, float yjs, int n)
{
    if (n<= 0)
    {
        float x = x0+(xis+yis)/2;
        float y = y0+(xjs+yjs)/2; 
        verticies.push_back(vec4(x, y, 0.0f, 1.0f));
    }    
    else
    {
        hilbert(x0, y0, yis/2, yjs/2, xis/2, xjs/2, n-1);
        hilbert(x0+xis/2, y0+xjs/2 ,xis/2, xjs/2, yis/2, yjs/2, n-1);
        hilbert(x0+xis/2+yis/2, y0+xjs/2+yjs/2, xis/2, xjs/2, yis/2, yjs/2,n-1);
        hilbert(x0+xis/2+yis, y0+xjs/2+yjs, -yis/2,-yjs/2, -xis/2, -xjs/2,n-1);
    }
}