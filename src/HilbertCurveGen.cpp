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

vector<float> HilbertCurveGen::getPointVerticies()
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

vector<float> HilbertCurveGen::getTriangleVerticies()
{
    vector<float> vert;
    vec3 xAxis(1.0f, 0.0f, 0.0f);
    float offSet = 0.01f;
    float reUsedX;
    float reUsedY;
    float reUsedZ;

    for (int i = 0; i < verticies.size() - 1; i++)
    {
        vec3 prevDist;
        if (i > 0)
        {
            // coords from point-based hilbert curve
            vert.push_back(verticies[i].x);
            vert.push_back(verticies[i].y);
            vert.push_back(verticies[i].z);

            prevDist = vec3(verticies[i] - verticies[i-1]);
            if (isOrthogonal(prevDist, xAxis))
            {
                vert.push_back(verticies[i].x);
                vert.push_back(verticies[i].y + offSet);
                vert.push_back(verticies[i].z);
            }
            else 
            {
                vert.push_back(verticies[i].x + offSet);
                vert.push_back(verticies[i].y);
                vert.push_back(verticies[i].z);
            }

            // 3rd point to complete the traingle and makes a rectangle
            vert.push_back(reUsedX);
            vert.push_back(reUsedY);
            vert.push_back(reUsedZ);
                        
        }

        // coords from point-based hilbert curve
        vert.push_back(verticies[i].x);
        vert.push_back(verticies[i].y);
        vert.push_back(verticies[i].z);

        // check to see if the line is horizontal or vertical
        // so we know where to put the trianlgle verticies
        vec3 nextDist = vec3(verticies[i]) - vec3(verticies[i+1]);
        if (isOrthogonal(nextDist, xAxis))
        {
            reUsedX = verticies[i].x;
            reUsedY = verticies[i].y + offSet;
            reUsedZ = verticies[i].z;
            vert.push_back(verticies[i].x);
            vert.push_back(verticies[i].y + offSet);
            vert.push_back(verticies[i].z);
        }
        else 
        {
            reUsedX = verticies[i].x + offSet;
            reUsedY = verticies[i].y;
            reUsedZ = verticies[i].z;
            vert.push_back(verticies[i].x + offSet);
            vert.push_back(verticies[i].y);
            vert.push_back(verticies[i].z);
        }

        // coords of next point to complete the triangle
        vert.push_back(verticies[i+1].x);
        vert.push_back(verticies[i+1].y);
        vert.push_back(verticies[i+1].z);
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

bool HilbertCurveGen::isOrthogonal(vec3 x, vec3 y)
{
    return dot(x, y);
}