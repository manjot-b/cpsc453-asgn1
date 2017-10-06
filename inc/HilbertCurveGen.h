#ifndef HILBERT_CURVE_GEN
#define HILBERT_CURVE_GEN

#include <vector>
#include <glm/glm.hpp>


class HilbertCurveGen
{
    public: 
		/*
			Generates a hilbert curve up to the level specified
			Parameters:
				level:	The level to generate the hilbert curve up to
				width:	The width of the screen in pixels
				height:	The height of the screen in pixels
		*/
        HilbertCurveGen(int level, int width, int height);
		
		/*
			Returns the verticies of each point with
			the current generated hilbert curve
		*/
        std::vector<float> getPointVerticies();

		/*
			Returns the verticies of the triangle that
			cover each line. The lines will be in the center
			of the triangle and each triangle has 3 verticies
		*/
        std::vector<float> getTriangleVerticies();

		/*
			Increases the level of the hilbert curve by 1
			as long as the next level is below the max level
		*/
        void increaseLevel();

		/*
			Decreases the level of the hilbert curve by 1
			as long as the next level is greater than 0
		*/
        void decreaseLevel();

		/*
			Call this function when the screen size changes
			so that a new max level can be calculated based
			on the new screen resolution
		*/
        void setMaxLevel(int width, int height);
    private:
        int level;		// the current level      
        int maxLevel;	// the max level
        std::vector<glm::vec4> verticies;	// the point verticies of the hilbert curve
        glm::mat4 lowerLeftTrans;	// transformation that brings curve to lower left
        glm::mat4 upperLeftTrans;	// transformation that brings curve to upper left
        glm::mat4 upperRightTrans;	// transformation that brings curve to upper right
        glm::mat4 lowerRightTrans;	// transformation that brings curve to lower right
        
		/*
			This will generate a hilbert curve that is
			n levels above the one in verticies
			Input:
				verticies:	The current set of points defining a hilbert curve at some level
				n:			The amount of levels to increase the hilbert curve in verticies
			Output:
				verticies:	Returns the new hilbert curve increased by n levels
		*/
        void hilbert(std::vector<glm::vec4> &verticies, int n);

		/*
			Returns weather two vectors are orthogonal
			Input:
				x:	The first vector
				y:	The second vector
			Output:
				bool: true if the vectors x and y are orthogonal
		*/
        bool isOrthogonal(glm::vec3 x, glm::vec3 y);

		/*
			Set's the precision of a float
			Input:
				x:			The float to set the precision of
				precision:	The precision to set for float
		*/
        float setFloatPrecision(float x, int precision);
        
};

#endif