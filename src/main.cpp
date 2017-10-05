#include <glad/glad.h>
//#define GLFW_INCLUDE_GLCOREARB
//#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Shader.h"
#include "HilbertCurveGen.h"

using namespace std;

HilbertCurveGen hbCurve(1);
vector<float> curveVer;
bool curveChanged = false;
bool n_key_held = false;
bool b_key_held = false;
bool space_key_held = false;
GLenum primitiveMode = GL_LINE_STRIP;

const int WIDTH = 512;
const int HEIGHT = 512;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !n_key_held)
	{
		n_key_held = true;
		hbCurve.increaseLevel();
		switch(primitiveMode)
		{
			case GL_LINE_STRIP:
				curveVer = hbCurve.getPointVerticies(); break;
			case GL_TRIANGLES:
				curveVer = hbCurve.getTriangleVerticies(); break;	
		}
		curveChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE)
		n_key_held = false;

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !b_key_held)
	{
		b_key_held = true;
		hbCurve.decreaseLevel();
		switch(primitiveMode)
		{
			case GL_LINE_STRIP:
				curveVer = hbCurve.getPointVerticies(); break;
			case GL_TRIANGLES:
				curveVer = hbCurve.getTriangleVerticies(); break;	
		}
		curveChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		b_key_held = false;
	
	// switch modes	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !space_key_held)
	{
		space_key_held = true;
		switch(primitiveMode)
		{
			case GL_LINE_STRIP:
				primitiveMode = GL_TRIANGLES;
				curveVer = hbCurve.getTriangleVerticies(); 
				break;
			case GL_TRIANGLES:
				primitiveMode = GL_LINE_STRIP;
				curveVer = hbCurve.getPointVerticies(); 
				break;	
		}
		curveChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)	
		space_key_held = false;

}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hilbert Curve", NULL, NULL);

	if (!window)
	{
		cout << "Failed to create GLFW window. TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	Shader shader("rsc/vertex.glsl", "rsc/fragment.glsl");
	curveVer = hbCurve.getPointVerticies();

	GLuint VAO, VBO;
	glGenBuffers(1, &VBO); // gen 1 buffer and store id in VBO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, curveVer.size() * sizeof(float), curveVer.data(), GL_STATIC_DRAW);

	const int COLOR_COMPONENTS = 0;
	const int VERTEX_COMPONENTS = 3;
	const int COMPONENTS_PER_VERTEX = COLOR_COMPONENTS + VERTEX_COMPONENTS;
	const int STRIDE = COMPONENTS_PER_VERTEX * sizeof(float);
	//position attributes
	glVertexAttribPointer(0, VERTEX_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);
	glEnableVertexAttribArray(0);
	//color attributes
	//glVertexAttribPointer(1, COLOR_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE, (void*)(VERTEX_COMPONENTS * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader.getProgramID());
		// update the verticies of the triangles
		glBindVertexArray(VAO);
		
		//create new buffer data
		if (curveChanged)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, curveVer.size() * sizeof(float), curveVer.data(), GL_STATIC_DRAW);
			curveChanged = false;	
		}
		
		glDrawArrays(primitiveMode, 0, curveVer.size() / 3);
		//glDrawArrays(GL_POINTS, 0, curveVer.size() / 3);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

