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
    // verticies for level 1
    verticies = 
    {
        vec4(-.5f, -.5f, 0.0f, 1.0f),
        vec4(-.5f, .5f, 0.0f, 1.0f),
        vec4(.5f, .5f, 0.0f, 1.0f),
        vec4(.5f, -.5f, 0.0f, 1.0f)
    };
   
 
    float scalarFl = .5f;
    float translateFl = .5f;
    lowerLeftTrans = translate(lowerLeftTrans, vec3(-translateFl, -translateFl, 0.0f));    
    lowerLeftTrans = rotate(lowerLeftTrans, radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));   // proper oreintation
    lowerLeftTrans = rotate(lowerLeftTrans, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));   //y-axis flip
    lowerLeftTrans = scale(lowerLeftTrans, vec3(scalarFl, scalarFl, scalarFl));
   
    upperLeftTrans = translate(upperLeftTrans, vec3(-translateFl, translateFl, 0.0f));  // already correct oreintation
    upperLeftTrans = scale(upperLeftTrans, vec3(scalarFl, scalarFl, scalarFl));
 
    upperRightTrans = translate(upperRightTrans, vec3(translateFl, translateFl, 0.0f)); // already correct oreintation
    upperRightTrans = scale(upperRightTrans, vec3(scalarFl, scalarFl, scalarFl));
 
    lowerRightTrans = translate(lowerRightTrans, vec3(translateFl, -translateFl, 0.0f));    
    lowerRightTrans = rotate(lowerRightTrans, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));  // proper oreintation
    lowerRightTrans = rotate(lowerRightTrans, radians(180.0f), vec3(0.0f, 1.0f, 0.0f)); //y-axis flipe    
    lowerRightTrans = scale(lowerRightTrans, vec3(scalarFl, scalarFl, scalarFl));
 
    hilbert(verticies, level-1);
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

    // level 1 verticies
    verticies = 
    {
        vec4(-.5f, -.5f, 0.0f, 1.0f),
        vec4(-.5f, .5f, 0.0f, 1.0f),
        vec4(.5f, .5f, 0.0f, 1.0f),
        vec4(.5f, -.5f, 0.0f, 1.0f)
    };
    --level;
    hilbert(verticies, level-1);    // calc from level 1 up to new current level
}

/*
    Will return in curve the new verticies that are n levels
    above it
*/
void HilbertCurveGen::hilbert(vector<vec4> &curve,  int n)
{
    if (n<= 0) { return; }    
    else
    {
        vector<vec4> lowerLeftCurve;
        vector<vec4> upperLeftCurve;
        vector<vec4> upperRightCurve;
        vector<vec4> lowerRightCurve;        
        for (const auto &v : curve) // translate every vertex and place in proper vector
        {
            lowerLeftCurve.push_back(lowerLeftTrans * v);
            upperLeftCurve.push_back(upperLeftTrans * v);
            upperRightCurve.push_back(upperRightTrans * v);
            lowerRightCurve.push_back(lowerRightTrans * v);
        }
        curve.clear();
        // order is important here
        curve.insert(curve.end(), lowerLeftCurve.begin(), lowerLeftCurve.end());
        curve.insert(curve.end(), upperLeftCurve.begin(), upperLeftCurve.end());
        curve.insert(curve.end(), upperRightCurve.begin(), upperRightCurve.end());
        curve.insert(curve.end(), lowerRightCurve.begin(), lowerRightCurve.end());
        hilbert(curve, n-1);
        
    }
}