#include "HilbertCurveGen.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <iomanip>

using namespace glm;
using namespace std;

HilbertCurveGen::HilbertCurveGen(int _level, int width, int height) :level(_level) , lowerLeftTrans(1.0f), lowerRightTrans(1.0f),
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
 
    setMaxLevel(width, height);
    hilbert(verticies, level-1);
}

vector<float> HilbertCurveGen::getPointVerticies()
{
    vector<float> vert;
    cout << "LEVEL: " << level << endl;  
    int i = 1;  
    for (const auto & vec : verticies)
    {
        vert.push_back(vec.x);
        vert.push_back(vec.y);
        vert.push_back(vec.z);
        //cout <<"POINT: " << i 
         //   << " X: " << vec.x << " Y: " << vec.y << endl;
        i++; 
    }
    return vert;
}

vector<float> HilbertCurveGen::getTriangleVerticies()
{
    vector<float> vert;	// will hold the verticies of the triangle
    vec3 xAxis(1.0f, 0.0f, 0.0f);
    float offSet = 0.02f;	// size of rectangle away from the vertex on each side
    float reUsedX;
    float reUsedY;
    float reUsedZ;

    // will return a triangles that form rectangles
    // such that each line is encompased in the
    // center of the rectangle
    for (int i = 0; i < verticies.size() - 1; i++)
    {
        vec3 prevDist;	// used to see if line between verticies is horizontal or vertical
        int direction;  // -1 means left/down, +1 means right/up
		int precision = 4;	// used to set the precision of floats

		// gets the trianglef rom the current vertex to the previous one
        if (i > 0 || i == verticies.size() - 2)
        {
            prevDist = vec3(verticies[i] - verticies[i-1]);
			prevDist.x = setFloatPrecision(prevDist.x, precision);
			prevDist.y = setFloatPrecision(prevDist.y, precision);
            if (isOrthogonal(prevDist, xAxis))
            {
                direction = (verticies[i].y - verticies[i-1].y) 
                / abs(verticies[i].y - verticies[i-1].y);

                vert.push_back(verticies[i].x + offSet);
                vert.push_back(verticies[i].y + offSet * direction);
                vert.push_back(verticies[i].z);

                 // coords from point-based hilbert curve
                vert.push_back(verticies[i].x - offSet);
                vert.push_back(verticies[i].y + offSet * direction);
                vert.push_back(verticies[i].z);
            }
            else // is parallel to x-axis
            {
                direction = (verticies[i].x - verticies[i-1].x) 
                / abs(verticies[i].x - verticies[i-1].x);

                 // coords from point-based hilbert curve
                vert.push_back(verticies[i].x + offSet * direction);
                vert.push_back(verticies[i].y + offSet);
                vert.push_back(verticies[i].z);

                vert.push_back(verticies[i].x + offSet * direction);
                vert.push_back(verticies[i].y - offSet);
                vert.push_back(verticies[i].z);
            }

            // 3rd point to complete the traingle and makes a rectangle
            vert.push_back(reUsedX);
            vert.push_back(reUsedY);
            vert.push_back(reUsedZ);
                        
        }

       

        // check to see if the line is horizontal or vertical
        // so we know where to put the trianlgle verticies
    	vec3 nextDist = vec3(verticies[i].x, verticies[i].y, verticies[i].z) -
			vec3(verticies[i + 1].x, verticies[i + 1].y, verticies[i + 1].z);
		nextDist.x = setFloatPrecision(nextDist.x, precision);
		nextDist.y = setFloatPrecision(nextDist.y, precision);
            
		// draws a triangle from the currnt vertex to the
		// next vertex
        if (isOrthogonal(nextDist, xAxis))
        {
            direction = (verticies[i+1].y - verticies[i].y) 
                    / abs(verticies[i+1].y - verticies[i].y);
            
            reUsedX = verticies[i].x - offSet;
            reUsedY = verticies[i].y;
            reUsedZ = verticies[i].z;

            // coords from point-based hilbert curve
            vert.push_back(verticies[i].x - offSet);
            vert.push_back(verticies[i].y);
            vert.push_back(verticies[i].z);
            
            vert.push_back(verticies[i].x + offSet);
            vert.push_back(verticies[i].y);
            vert.push_back(verticies[i].z);

            // coords of next point to complete the triangle
            vert.push_back(verticies[i+1].x + offSet);
            vert.push_back(verticies[i+1].y + offSet * direction);
            vert.push_back(verticies[i+1].z);
        }
        else // parallel to x-axis
        {
            direction = (verticies[i+1].x - verticies[i].x) 
            / abs(verticies[i+1].x - verticies[i].x);

            reUsedX = verticies[i].x;
            reUsedY = verticies[i].y - offSet;
            reUsedZ = verticies[i].z;

            vert.push_back(verticies[i].x);
            vert.push_back(verticies[i].y + offSet);
            vert.push_back(verticies[i].z);

            vert.push_back(verticies[i].x);
            vert.push_back(verticies[i].y - offSet);
            vert.push_back(verticies[i].z);
        
            // coords of next point to complete the triangle
            vert.push_back(verticies[i+1].x + offSet * direction);
            vert.push_back(verticies[i+1].y + offSet);
            vert.push_back(verticies[i+1].z);
        }

		// on second last point, so draw the triangle
		// from the last point to the current point
        if (i == verticies.size() - 2) 
        {
            prevDist = vec3(verticies[i+1] - verticies[i]);
			prevDist.x = setFloatPrecision(prevDist.x, precision);
			prevDist.y = setFloatPrecision(prevDist.y, precision);
            if (isOrthogonal(prevDist, xAxis))
            {
                direction = (verticies[i+1].y - verticies[i].y) 
                / abs(verticies[i+1].y - verticies[i].y);

                vert.push_back(verticies[i+1].x + offSet);
                vert.push_back(verticies[i+1].y + offSet * direction);
                vert.push_back(verticies[i+1].z);

                 // coords from point-based hilbert curve
                vert.push_back(verticies[i+1].x - offSet);
                vert.push_back(verticies[i+1].y + offSet * direction);
                vert.push_back(verticies[i+1].z);
            }
            else // is parallel to x-axis
            {
                direction = (verticies[i+1].x - verticies[i].x) 
                / abs(verticies[i+1].x - verticies[i].x);

                 // coords from point-based hilbert curve
                vert.push_back(verticies[i+1].x + offSet * direction);
                vert.push_back(verticies[i+1].y + offSet);
                vert.push_back(verticies[i+1].z);

                vert.push_back(verticies[i+1].x + offSet * direction);
                vert.push_back(verticies[i+1].y - offSet);
                vert.push_back(verticies[i+1].z);
            }

            // 3rd point to complete the traingle and makes a rectangle
            vert.push_back(reUsedX);
            vert.push_back(reUsedY);
            vert.push_back(reUsedZ);
        }
    }
    return vert;
}

void HilbertCurveGen::increaseLevel()
{
    if (level >= maxLevel)
    {
        cout << "MAX LEVEL " << maxLevel << " REACHED!" << endl;
        return;
    }
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


void HilbertCurveGen::hilbert(vector<vec4> &curve,  int n)
{
    if (n<= 0) { return; }    
    else
    {
        vector<vec4> lowerLeftCurve;
        vector<vec4> upperLeftCurve;
        vector<vec4> upperRightCurve;
        vector<vec4> lowerRightCurve;        
        for (const auto &v : curve) // transform every vertex and place in proper vector
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
    if (dot(x, y) == 0) return true;
    return false;
}

void HilbertCurveGen::setMaxLevel(int width, int height)
{
    float area = width * height;
    maxLevel = (1 / 2.0f) * (log(area) / log(2));
}

float HilbertCurveGen::setFloatPrecision(float x, int precision)
{
    int val = x * pow(10, precision);
    val = round(val);
    float prec = val / pow(10, precision);
    return prec;
}