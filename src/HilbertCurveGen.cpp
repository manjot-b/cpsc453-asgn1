#include "HilbertCurveGen.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace glm;
using namespace std;

HilbertCurveGen::HilbertCurveGen(int _level) :level(_level) , lowerLeftTrans(1.0f), lowerRightTrans(1.0f),
                upperLeftTrans(1.0f), upperRightTrans(1.0f)
{
    verticies = 
    {
        vec4(-.5f, -.5f, 0.0f, 1.0f),
        vec4(-.5f, .5f, 0.0f, 1.0f),
        vec4(.5f, .5f, 0.0f, 1.0f),
        vec4(.5f, -.5f, 0.0f, 1.0f)
    };
    //lowerLeftTrans = vec3(1.0f);
    //lowerRight = vec3(1.0f);
    //upperLeft = vec3(1.0f);
    //upperRight = vec3(1.0f);

    float yAxisFlipArray[16] = {
        -1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    };
    mat4 yAxisFlip = glm::make_mat4(yAxisFlipArray);
    float scalarFl = .5f;
    float translateFl = .5f;
    lowerLeftTrans = translate(lowerLeftTrans, vec3(-translateFl, -translateFl, 0.0f));    
    lowerLeftTrans = rotate(lowerLeftTrans, radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));
    lowerLeftTrans = lowerLeftTrans * yAxisFlip;
    lowerLeftTrans = scale(lowerLeftTrans, vec3(scalarFl, scalarFl, scalarFl));
   
    upperLeftTrans = translate(upperLeftTrans, vec3(-translateFl, translateFl, 0.0f));    
    //upperLeftTrans = rotate(upperLeftTrans, radians(180.0f), vec3(0.0f, 0.0f, 1.0f));
    upperLeftTrans = scale(upperLeftTrans, vec3(scalarFl, scalarFl, scalarFl));
 
    upperRightTrans = translate(upperRightTrans, vec3(translateFl, translateFl, 0.0f));    
    //upperRightTrans = rotate(upperRightTrans, radians(-180.0f), vec3(0.0f, 0.0f, 1.0f));
    upperRightTrans = scale(upperRightTrans, vec3(scalarFl, scalarFl, scalarFl));
 
    lowerRightTrans = translate(lowerRightTrans, vec3(translateFl, -translateFl, 0.0f));    
    lowerRightTrans = rotate(lowerRightTrans, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
    lowerRightTrans = lowerRightTrans * yAxisFlip;    
    lowerRightTrans = scale(lowerRightTrans, vec3(scalarFl, scalarFl, scalarFl));
 
    //hilbert(verticies, level);
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
    hilbert(verticies, 1);
    level++;
}

void HilbertCurveGen::decreaseLevel()
{
    if (level <= 1) return;
    verticies = 
    {
        vec4(-.5f, -.5f, 0.0f, 1.0f),
        vec4(-.5f, .5f, 0.0f, 1.0f),
        vec4(.5f, .5f, 0.0f, 1.0f),
        vec4(.5f, -.5f, 0.0f, 1.0f)
    };
    --level;
    hilbert(verticies, level-1);
}

/* x0 and y0 are the coordinates of the bottom left corner */
/* xis & xjs are the i & j components of the unit x vector this frame */
/* similarly yis and yjs */
void HilbertCurveGen::hilbert(vector<vec4> &curve,  int n)
{
    if (n<= 0) { return; }    
    else
    {
        vector<vec4> lowerLeftCurve;
        vector<vec4> upperLeftCurve;
        vector<vec4> upperRightCurve;
        vector<vec4> lowerRightCurve;        
        for (const auto &v : curve)
        {
            lowerLeftCurve.push_back(lowerLeftTrans * v);
            upperLeftCurve.push_back(upperLeftTrans * v);
            upperRightCurve.push_back(upperRightTrans * v);
            lowerRightCurve.push_back(lowerRightTrans * v);
        }
        curve.clear();
        curve.insert(curve.end(), lowerLeftCurve.begin(), lowerLeftCurve.end());
        curve.insert(curve.end(), upperLeftCurve.begin(), upperLeftCurve.end());
        curve.insert(curve.end(), upperRightCurve.begin(), upperRightCurve.end());
        curve.insert(curve.end(), lowerRightCurve.begin(), lowerRightCurve.end());
        hilbert(curve, n-1);
        
    }
}